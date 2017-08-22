#!/usr/bin/env python
#===------------------------------------------------------------------------===#
# 
# This script is used for parsing the dump from O2codecheck checks.
# When an error is dumped, it is translated to a key-value pair that accordingly correspond to
# the problematic part that could not be fixed and the string with which the part should be replaced.
# 
#===------------------------------------------------------------------------===#
# 
import sys
import re
import os

def log_error_names(clang_tidy_output, configs_dir):
  check_error_names_set = set(())

  number_of_errors = 0
  for line in clang_tidy_output.split('\n'):
    if ' error:' not in line:
      continue

    m = re.search('.*Could not fix \'(.+)\'.*\[(.+)\].*', line)
    error_name = m.group(1)
    check_name = m.group(2)
    
    if( (check_name, error_name) in check_error_names_set ):
      continue
    check_error_names_set.add((check_name, error_name))
    
    with open(os.path.join(configs_dir, check_name), 'a') as f:
      f.write("{key: '%s.%s', value: <provide_fix_for_key>}\n" % (check_name, error_name))
      number_of_errors = number_of_errors + 1
      
  print 'Number of errors logged:', number_of_errors

def main():
  configs_dir = './configs/'
  
  clang_tidy_output = ''
  
  for line in sys.stdin:
    clang_tidy_output += line

  log_error_names(clang_tidy_output, configs_dir)
     

if __name__ == '__main__':
  main()
