#! /usr/bin/env python3
from os import listdir
from os.path import dirname, basename, isfile, join
import data

from inspect import getmembers
from copy import deepcopy

from pathlib import Path
import importlib

modules = Path('.').glob(join(dirname(__file__), "*.py"))
module_dir = dirname(__file__)
modules = listdir(module_dir)
__all__ = [ basename(f)[:-3] for f in modules if isfile(join(module_dir, f)) and f[-3:] == '.py' and not f=='__init__.py']


def compute_math_channels():
	prefix = 'math_'

	data.update_math_table_count()

	for module_name in __all__:
		module = importlib.import_module('math_functions.'+module_name)
		members_of_current_module = getmembers(module)
		for member in members_of_current_module:
			if member[0].find(prefix) == 0:
				try:
					member[1]() # { fcn_name : fcn_obj }
				except:
					print("Error occurred during math computation: {}".format(member[0]))

	return
