#! /usr/bin/env python3
from data import Channel
import numpy as np

def math_gps_linvelz():
	local_z = Channel("vehicle_local_position_0", "z")

	dz = np.diff(local_z[:])
	dt = np.diff(local_z.timedata())
	local_vz = np.divide(dz, dt) * 1E06 # m/us -> m/s
	local_vz = np.insert(local_vz, 0, local_vz[0])

	math_gps_linvelz = Channel()
	math_gps_linvelz.set_data(local_vz)
	math_gps_linvelz.set_timedata(local_z.timedata())
	math_gps_linvelz.set_timechannel_id(local_z.get_timechannel_id())
	math_gps_linvelz.set_unit_string("m/s")
	math_gps_linvelz.write("gps_linvelz")
