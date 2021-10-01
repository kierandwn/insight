#! /usr/bin/env python3
from contextlib import closing

import sqlite3 as sql
import numpy as np

db_name = 0
layer = 0
n_math_tables = 0

def submit_query(sql_query, all=False):
	global db_name
	with closing(sql.connect(db_name)) as con, con, closing(con.cursor()) as cur:
		cur.execute(sql_query)
		if all:
			return cur.fetchall()
		else:
			return cur.fetchone()
 
def is_hid_in_layer(hid):
	global layer
	query = "SELECT count(*) FROM layers_table WHERE layer={} AND string_id='{}'".format(layer, hid)
	count = submit_query(query)
	return count[0]

def tid_from_hid(hid):
	global layer
	query = "SELECT table_id FROM layers_table WHERE layer={} AND string_id='{}'".format(layer, hid)
	tid = submit_query(query)
	return tid[0]

def get_time_channel_id(tid):
	query = "SELECT independent_variable FROM files WHERE table_id='{}'".format(tid)
	ivarid = submit_query(query)
	return ivarid[0]

def get_channel_data(table_id, channel_id):
	result = np.array([])
	query = "SELECT {} FROM data_{}".format(channel_id, table_id)

	for row in submit_query(query, all=True):
		result = np.append(result, row[0])
	return result

def update_math_table_count():
	global n_math_tables
	query = "SELECT count(*) FROM math_tables"
	result = submit_query(query)
	n_math_tables = result[0]

def add_math_channel(math_channel_id, independent_variable_id):
	global n_math_tables, layer
	n_math_tables += 1

	query = "INSERT INTO math_tables VALUES({}, {}, '{}', '{}')".format( \
		layer,
		n_math_tables, \
		math_channel_id, \
		independent_variable_id \
	)
	submit_query(query)

	query = "CREATE TABLE math_{} (idx INT PRIMARY KEY, {} DOUBLE, {} DOUBLE)".format( \
		n_math_tables, \
		math_channel_id, \
		independent_variable_id \
	)
	submit_query(query)
	return n_math_tables

def add_mathtable_data(mid, data, tdata):
	query = "INSERT INTO math_{} VALUES (0, {}, {})".format( \
		mid, \
		data[0], \
		tdata[0] \
	)
	for i in range(1, len(data)):
		query += ",({}, {}, {})".format(i, data[i], tdata[i])

	submit_query(query)

def add_math_unit_string(math_table_sid, channel_id, unit_string):
	query = "INSERT INTO unit_table VALUES ('{}', '{}', '{}')".format( \
		"math::{}".format(math_table_sid), \
		channel_id, \
		unit_string \
	)
	submit_query(query)

def set_db_filepath(db_filepath):
	global db_name
	db_name = db_filepath


class Channel:
	def __init__(self, table_hid="", channel_id=""):
		self.data  = np.array([])
		self.tdata = np.array([])
		self.read_from_db = False
		self.ivarid = "t"
		self.unit_string = "-"

		if not (table_hid=="" and channel_id==""):
			self.read(table_hid, channel_id)

	def read(self, table_hid, channel_id):
		if is_hid_in_layer(table_hid):
			tid = tid_from_hid(table_hid)
			ivarid = get_time_channel_id(tid)

			self.data  = get_channel_data(tid, channel_id)
			self.tdata = get_channel_data(tid, ivarid)
			self.ivarid = ivarid
			self.read_from_db = True

	def get_timechannel_id(self):
		return self.ivarid

	def set_data(self, data):
		self.data = data

	def set_timedata(self, tdata):
		self.tdata = tdata

	def set_timechannel_id(self, ivarid):
		self.ivarid = ivarid

	def set_unit_string(self, unit_string):
		self.unit_string = unit_string

	def write(self, math_channel_id):
		mid = add_math_channel(math_channel_id, self.ivarid)
		add_mathtable_data(mid, self.data, self.tdata)
		add_math_unit_string(math_channel_id, math_channel_id, self.unit_string)

	def timedata(self):
		return self.tdata

	def __getitem__(self, *args):
		return self.data.__getitem__(*args)

	def __setitem__(self, *args):
		self.data.__setitem__(*args)

	def __delitem__(self, *args):
		self.data.__delitem__(*args)

	def __getslice__(self, *args):
		return self.__getslice__(*args)

	def __setslice__(self, *args):
		self.data.__setslice__(*args)

	def __delslice__(self, *args):
		self.data.__delslice__(*args)

	def __add__(self, rhs):
		result = Channel()
		result.set_data(rhs + self.data) # reversed so that rhs Channels are also supported
		return result

	def len(self):
		return len(self.data)




