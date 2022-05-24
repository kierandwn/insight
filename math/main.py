#! /usr/bin/env python3
import sys
import math_functions as fn

if __name__ == '__main__' and len(sys.argv) > 1:
	db_filepath = sys.argv[1]
	layer = sys.argv[2]

	print("layer: {}".format(layer))

	fn.data.set_db_filepath(db_filepath)
	fn.data.set_db_active_layer(layer)

	fn.compute_math_channels()