#! /usr/bin/env python3
import sqlite3 as sql
import numpy as np

db = 0
layer = 0
n_math_tables = 0
 
def is_hid_in_layer(hid):
	global db, layer
	query = "SELECT count(*) FROM layer_{} WHERE string_id='{}'".format(layer, hid)
	c = db.cursor()
	c.execute(query)

	count = c.fetchone()
	return count[0]

def tid_from_hid(hid):
	global db, layer
	query = "SELECT table_id FROM layer_{} WHERE string_id='{}'".format(layer, hid)
	c = db.cursor()
	c.execute(query)

	tid = c.fetchone()
	return tid[0]

def get_time_channel_id(tid):
	global db
	query = "SELECT independent_variable FROM files WHERE table_id='{}'".format(tid)
	c = db.cursor()
	c.execute(query)

	ivarid = c.fetchone()
	return ivarid[0]

def get_channel_data(table_id, channel_id):
	global db
	result = np.array([])
	query = "SELECT {} FROM data_{}".format(channel_id, table_id)
	c = db.cursor()

	for row in c.execute(query):
		result = np.append(result, row[0])
	return result

def update_math_table_count():
	global db, n_math_tables
	query = "SELECT count(*) FROM math_tables"
	c = db.cursor()
	c.execute(query)
	n_math_tables = c.fetchone()[0]

def add_math_channel(math_channel_id, independent_variable_id):
	global db, n_math_tables
	n_math_tables += 1

	query = "INSERT INTO math_tables VALUES({}, '{}', '{}')".format( \
		n_math_tables, \
		math_channel_id, \
		independent_variable_id \
	)
	db.execute(query)

	query = "CREATE TABLE math_{} (idx INT PRIMARY KEY, {} DOUBLE, {} DOUBLE)".format( \
		n_math_tables, \
		math_channel_id, \
		independent_variable_id \
	)
	db.execute(query)
	return n_math_tables

def add_mathtable_data(mid, data, tdata):
	query = "INSERT INTO math_{} VALUES (0, {}, {})".format( \
		mid, \
		data[0], \
		tdata[0] \
	)
	for i in range(1, len(data)):
		query += ",({}, {}, {})".format(i, data[i], tdata[i])

	db.execute(query)

def open(db_filepath):
	global db
	db = sql.connect(db_filepath)
	update_math_table_count()

def close():
	global db
	db.commit()
	db.close()

class Channel:
	def __init__(self, table_hid="", channel_id=""):
		self.data  = np.array([])
		self.tdata = np.array([])
		self.read_from_db = False
		self.ivarid = "t"

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

	def write(self, math_channel_id):
		mid = add_math_channel(math_channel_id, self.ivarid)
		add_mathtable_data(mid, self.data, self.tdata)

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




