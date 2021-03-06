#! /usr/bin/env python
#
# Copyright (c) 2012 Bryce Adelstein-Lelbach
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# TODO: Fractional threads_per_locality

# hpx_run_test.py is a script that is designed to manage the execution of HPX
# tests. It is designed to:
#
# * Robustly clean up failed tests by killing them and all their subprocesses.
# * Robustly enforce user-specified timeouts for tests with minimal overheads.
# * Control the output of tests.
#
# hpx_run_test.py takes a number of command line arguments, described in
# --help. Additionally, it takes a positional argument that describes
# the test. This argument should be formatted as a python list. Its format is
# described in --help.

import sys, os, string
import os.path as osp

from types import StringType

from optparse import OptionParser

from errno import ENOENT

import signal, re

if osp.exists(osp.join(sys.path[0], "../hpx")):
  sys.path.append(osp.join(sys.path[0], ".."))
if osp.exists(osp.join(sys.path[0], "../share/hpx/python/hpx")):
  sys.path.append(osp.join(sys.path[0], "../share/hpx/python"))

from hpx.process import process, process_group

signal_map = dict((-1 * k, v) for v, k in signal.__dict__.iteritems() \
                           if re.match("SIG[A-Z]*$", v))

def exit_decode(exit_code):
  # In Bash on Linux, programs exit with a code of -1 * signal when an unhandled
  # signal occurs.
  if exit_code < 0 and exit_code in signal_map:
    return signal_map[exit_code]
  else:
    return exit_code

def quote_options(options, quoting_char = '"'):
  no_quote = string.letters + string.digits + '-+=/_.'
  s = ''

  for option in options:
    if type(option) is not StringType:
      option = str(option)
    for c in option:
      if c not in no_quote:
        s += ' ' + quoting_char + option + quoting_char
        break
    else:
      s += ' ' + option

  return string.strip(s)

class TestFailed(Exception):
  pass

if __name__ == '__main__':
  # {{{ main
  usage =         "Usage: %prog [options] [test specification]\n"
  usage = usage + "The test specification should be a python list the following format:\n"
  usage = usage + "  [ name, timeout, success, nodes, threads_per_node, args ]\n"
  usage = usage + "The types of the list elements should be:\n"
  usage = usage + "  [ string, float or None, bool, int, int, list ]\n"

  parser = OptionParser(usage=usage)

  parser.add_option("--suffix",
                    action="store", type="string",
                    dest="suffix", default="_test",
                    help="Suffix added to test names [default: %default]")

  parser.add_option("--launcher",
                    action="store", type="string",
                    dest="launcher", default="",
                    help="Program used to launch the test [default: %default]")

  parser.add_option("--args",
                    action="store", type="string",
                    dest="args", default="",
                    help="Command line arguments to add tests [default: %default]")

  parser.add_option("--log",
                    action="store", type="string",
                    dest="log", default="fail",
                    help="Always log output (--log=always), never log "
                        +"output (--log=never) or log output for tests "
                        +"that fail (--log=fail) [default: %default]")

  parser.add_option("--log-stdout",
                    action="store_true", dest="log_stdout", default=False,
                    help="Send logs to stdout (overrides --log-prefix)")

  parser.add_option("--log-prefix",
                    action="store", type="string",
                    dest="log_prefix", default="./",
                    help="Prefix for log files [default: %default]")

  parser.add_option("--no-exit-code",
                    action="store_false", dest="exit_code", default=True,
                    help="Don't return a non-zero exit code when tests fail")

  (options, files) = parser.parse_args()

  if 'HPX_TEST_ARGUMENTS' in os.environ:
    options.args += os.environ['HPX_TEST_ARGUMENTS']

  if 'HPX_TEST_LAUNCHER' in os.environ:
    options.launcher += os.environ['HPX_TEST_LAUNCHER']

  if not (lambda x: "always" == x or "never" == x or "fail" == x)(options.log):
    print "Error: --log=" + quote_options([options.log]) + " is invalid\n"
    parser.print_help()
    sys.exit(1)

  if 0 == len(files):
    print "Error: test specified\n"
    parser.print_help()
    sys.exit(1)

  tests = []
  all_passed = True

  for f in files:
    tests += eval(f)#eval(open(f).read())

  for [name, timeout, success, nodes, threads_per_node, args] in tests:
    print "Running: " + name + " (Timeout:", timeout, "[s])",
    sys.stdout.flush()

    pg = process_group()
    results = [] # [ cmd, cmd_passed, exit_code, timed_out, output ]
    cmds = {}

    if not osp.exists(name):
      print "-", "Failed (test not found)"

      all_passed = False

      if "always" == options.log or "fail" == options.log:
        f = None

        if not options.log_stdout:
          log = name + ".log"
          f = open(log, "w+")
          print (" " * 2) + "Log:", log
        else:
          f = sys.stdout

        print >> f, ("#" * 80)
        print >> f, "Test:", name
        print >> f, "Result: Failed (test not found)"
        print >> f, ("#" * 80)

      continue

    for node in range(nodes):
      cmd = []

      if options.launcher:
        cmd = [options.launcher]

      cmd += [ name
             , '-t' + str(threads_per_node)]

      if nodes > 1:
          cmd += [ '-l' + str(nodes)
                 , '-' + str(node)]

      if options.args:
        cmd += [options.args]

      cmd += args

      cmd = quote_options(cmd)

      cmds[pg.create_process(cmd).fileno()] = cmd

    def gather_results(fd, job, output):
      cmd_passed = (job.poll() == 0 if success else job.poll() != 0) \
           and not job.timed_out()

      results.append([ cmds[job.fileno()]
                     , cmd_passed
                     , job.poll()
                     , job.timed_out()
                     , output])

      if not cmd_passed:
        raise TestFailed()

    try:
      pg.read_all(timeout, gather_results)
    except TestFailed:
      def read_callback(fd, job):
        try:
          gather_results(fd, job, job.read(0.5))
        except TestFailed:
          pass

      pg.terminate_all(read_callback)

    # all the commands are now done

    test_passed = True

    for result in results:
      if not result[1]:
        test_passed = False
        break

    all_passed = all_passed and test_passed

    print "-", ("Passed" if test_passed else "Failed")

    if "always" == options.log or ("fail" == options.log and not test_passed):
      f = None

      if not options.log_stdout:
        log = name + ".log"
        f = open(log, "w+")
        print (" " * 2) + "Log:", log
      else:
        f = sys.stdout

      print >> f, ("#" * 80)
      print >> f, "Test:", name
      print >> f, "Result:", ("Passed" if test_passed else "Failed")
      print >> f, ("#" * 80)
      print >> f, ""

      for result in results:
        print >> f, ("#" * 80)
        print >> f, "Command:", result[0]
        print >> f, "Result:", ("Passed" if result[1] else "Failed")
        print >> f, "Exit code:", exit_decode(result[2])
        print >> f, "Timed out:", result[3]
        print >> f, ("#" * 80)

        if result[4] is not None:
          if 0 != len(result[4]):
            print >> f, result[4],
            print >> f, ("#" * 80)

        print >> f, ""

  if not all_passed and options.exit_code:
    sys.exit(1)
  # }}}

