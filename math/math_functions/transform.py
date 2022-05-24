#! /usr/bin/env python3
import data

# for math channels, do not import single functions from other modules, import module and call as mod.func()
# import numpy as np
# import math

def math_att_from_quat():
    print("math_att_from_quat")
    # base_chan_logged = ['mros_localpose_quat', 'mros_setpointlocal_quat', 'mros_vio_quat']
    # base_chan_output = ['math_localpose', 'math_setpointlocal', 'math_vio']

    # for i in range(len(base_chan_logged)):
    #     base_chan_in = base_chan_logged[i]
    #     base_chan_out = base_chan_output[i]

    #     xchan = base_chan_in+'x'

    #     if xchan in data.keys():
    #         n = len(data[xchan])

    #         roll = np.zeros(n)
    #         pitch = np.zeros(n)
    #         yaw = np.zeros(n)

    #         for i in range(n):
    #             quat = [data[base_chan_in+'x'][i], data[base_chan_in+'y'][i], data[base_chan_in+'z'][i], \
    #                 data[base_chan_in+'w'][i]]

    #             rpy = quat_to_euler(quat)
    #             roll[i] = rpy[0]
    #             pitch[i] = rpy[1]
    #             yaw[i] = rpy[2]

    #         data[base_chan_out+'_roll'] = roll * (180/np.pi)
    #         data[base_chan_out+'_pitch'] = pitch * (180/np.pi)
    #         data[base_chan_out+'_yaw'] = yaw * (180/np.pi)

    # return data

def math_att_from_quat_vec():
    print("math_att_from_quat_vec")
    # base_chan_logged = ['px4_vehicle_attitude_0_q', 'px4_vehicle_attitude_groundtruth_0_q']
    # base_chan_output = ['px4_vehicle_attitude', 'px4_vehicle_attitude_groundtruth']

    # for i in range(len(base_chan_logged)):
    #     base_chan_in = base_chan_logged[i]
    #     base_chan_out = base_chan_output[i]

    #     xchan = base_chan_in+'[0]'

    #     if xchan in data.keys():
    #         n = len(data[xchan])

    #         roll = np.zeros(n)
    #         pitch = np.zeros(n)
    #         yaw = np.zeros(n)

    #         for i in range(n):
    #             quat = [data[base_chan_in+'[0]'][i], data[base_chan_in+'[1]'][i], data[base_chan_in+'[2]'][i], \
    #                 data[base_chan_in+'[3]'][i]]

    #             rpy = quat_to_euler(quat)
    #             roll[i] = rpy[0]
    #             pitch[i] = rpy[1]
    #             yaw[i] = rpy[2]

    #         data[base_chan_out+'_roll'] = roll * (180/np.pi)
    #         data[base_chan_out+'_pitch'] = pitch * (180/np.pi)
    #         data[base_chan_out+'_yaw'] = yaw * (180/np.pi)

    # return data

def quat_to_euler():
    print("quat_to_euler")
    # r = math.atan2(2*(q[0]*q[1]+q[2]*q[3]), 1-2*(q[1]**2+q[2]**2))
    # p = math.asin(2*(q[0]*q[2]-q[3]*q[1]))
    # y = math.atan2(2*(q[0]*q[3]+q[1]*q[2]), 1-2*(q[2]**2+q[3]**2))

    # return r, p, y

def math_gps_pos_lat_lon():
    print("math_gps_pos_lat_lon")
    # https://en.wikipedia.org/wiki/Haversine_formula

    # R = 6371000 # radius of the earth [m]
    # n = len(data['mros_gps_lat'])

    # lat = data['mros_gps_lat'] * (np.pi/180) # rad
    # lon = data['mros_gps_long'] * (np.pi/180)

    # # scaling in latitude
    # lam = np.cos(np.mean(lat))

    # lat0 = lat[0]
    # lon0 = lon[0]

    # dlat = lat-lat0
    # dlon = lon-lon0

    # data['math_gps_posx'] = R*dlon
    # data['math_gps_posy'] = R*dlat

    # return data

def math_setpointlocalatt_absyaw():
    print("math_setpointlocalatt_absyaw")
    # yaw_chan = 'math_setpointlocalatt_yaw'

    # if yaw_chan in data.keys():
    #     data['math_setpointlocal_absyaw'] = np.abs(data[yaw_chan])

    # return data

def math_gps_vel():
    print("math_gps_vel")
    # dx = np.diff(data['math_gps_posx'])
    # dy = np.diff(data['math_gps_posy'])

    # ds = np.sqrt(dx**2 + dy**2)
    # dt = np.diff(data['mission_time'])

    # data['math_gps_linvelx'] = np.insert(np.divide(dx, dt), 0, 0.0)
    # data['math_gps_linvely'] = np.insert(np.divide(dy, dt), 0, 0.0)
    # data['math_gps_linvelxy'] = np.insert(np.divide(ds, dt), 0, 0.0)
    # return data

def math_gps_linvelz():
    print("math_gps_linvelz")
    # dz = np.diff(data['mros_gps_alt'])
    # dt = np.diff(data['mission_time'])

    # data['math_gps_linvelz'] = np.insert(np.divide(dz, dt), 0, 0.0)
    # return data

def math_est_absvel():
    print("math_est_absvel")
    # dx = data['mros_odom_linvelx']
    # dy = data['mros_odom_linvely']

    # data['math_localpose_linvelxy'] = np.sqrt(dx**2 + dy**2)
    # return data
