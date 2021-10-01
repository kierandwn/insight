#! /usr/bin/env python3
import sys
import math_functions as fn

if __name__ == '__main__' and len(sys.argv) > 1:
	db_filepath = sys.argv[1]
	fn.data.set_db_filepath(db_filepath)
	fn.compute_math_channels()