#! /usr/bin/env python3
from data import Channel
import numpy as np

def quat2rotm(q):
    return np.array([
        [q[0]**2+q[1]**2-q[2]**2-q[3]**2, 2*(q[1]*q[2]+q[0]*q[3]), 2*(q[1]*q[3]-q[0]*q[2])],
        [2*(q[1]*q[2]-q[0]*q[3]), q[0]**2-q[1]**2+q[2]**2-q[3]**2, 2*(q[2]*q[3]+q[0]*q[1])],
        [2*(q[1]*q[3]+q[0]*q[2]), 2*(q[2]*q[3]-q[0]*q[1]), q[0]**2-q[1]**2-q[2]**2+q[3]**2]
    ])

def euler2rotm(theta1, theta2, theta3):
    return R1(theta3).dot(R2(theta2).dot(R3(theta1)))

# def quaternion(R):
#     _4q1q1 = 1 + R[0,0] + R[1,1] + R[2,2]
#     _4q2q2 = 1 + R[0,0] - R[1,1] - R[2,2]
#     _4q3q3 = 1 - R[0,0] + R[1,1] - R[2,2]
#     _4q4q4 = 1 - R[0,0] - R[1,1] + R[2,2]
    
#     i = np.argmax([_4q1q1, _4q2q2, _4q3q3, _4q4q4])
#     i = 0
    
#     if (i == 0):
#         _4q1q2 = R[1,2] - R[2,1]
#         _4q1q3 = R[2,0] - R[0,2]
#         _4q1q4 = R[0,1] - R[1,0]
        
#         _q1 = np.sqrt(0.25*_4q1q1)
#         q = np.array([
#             _q1,
#             0.25*_4q1q2/_q1,
#             0.25*_4q1q3/_q1,
#             0.25*_4q1q4/_q1
#         ])
#     elif (i == 1):
#         # error
#         _4q1q2 = R[1,2] - R[2,1]
#         _4q2q3 = R[0,1] + R[1,0]
#         _4q2q4 = R[2,0] + R[0,2]
        
#         _q2 = np.sqrt(0.25*_4q2q2)
#         q = np.array([
#             0.25*_4q1q2/_q2,
#             _q2,
#             0.25*_4q2q3/_q2,
#             0.25*_4q2q4/_q2
#         ])
#     elif (i == 2):
#         # error
#         _4q1q3 = R[2,0] - R[0,2]
#         _4q2q3 = R[0,1] + R[1,0]
#         _4q3q4 = R[1,2] + R[2,1]
    
#         _q3 = np.sqrt(0.25*_4q3q3)
#         q = np.array([
#             0.25*_4q1q3/_q3,
#             0.25*_4q2q3/_q3,
#             _q3,
#             0.25*_4q3q4/_q3
#         ])
#     elif (i == 3):
#         # error
#         _4q1q4 = R[0,1] - R[1,0]
#         _4q2q4 = R[2,0] + R[0,2]
#         _4q3q4 = R[1,2] + R[2,1]
        
#         _q4 = np.sqrt(0.25*_4q4q4)
#         q = np.array([
#             0.25*_4q1q4/_q4,
#             0.25*_4q2q4/_q4,
#             0.25*_4q3q4/_q4,
#             _q4
#         ])
        
#     return q


# def math_quat_to_rpy():
# 	q0 = Channel("vehicle_attitude_0", "q_0_")
# 	q1 = Channel("vehicle_attitude_0", "q_1_")
# 	q2 = Channel("vehicle_attitude_0", "q_2_")
# 	q3 = Channel("vehicle_attitude_0", "q_3_")

# 	r = np.empty()
# 	p = np.empty()
# 	y = np.empty()

# 	for i in range(q0.len()):
# 		q = np.array([q0[i], q1[i], q2[i], q3[i]])
# 		R = quat2rotm(q)
# 		r.append(np.arctan2(-1 * R[2][1], R[2][2]) * 180 / np.pi)
# 		p.append(np.arcsin(R[2][0]) * 180 / np.pi)
# 		y.append(np.arctan2(-1 * R[1][0], R[0][0]) * 180 / np.pi)

# 	math_quat_r = Channel()
# 	math_quat_r.set_data(r)
# 	math_quat_r.set_timedata(q0.timedata())
# 	math_quat_r.set_timechannel_id(q0.get_timechannel_id())
# 	math_quat_r.set_unit_string("deg")
# 	math_quat_r.write("roll")

# 	math_quat_p = Channel()
# 	math_quat_p.set_data(p)
# 	math_quat_p.set_timedata(q0.timedata())
# 	math_quat_p.set_timechannel_id(q0.get_timechannel_id())
# 	math_quat_p.set_unit_string("deg")
# 	math_quat_p.write("pitch")

# 	math_quat_y = Channel()
# 	math_quat_y.set_data(y)
# 	math_quat_y.set_timedata(q0.timedata())
# 	math_quat_y.set_timechannel_id(q0.get_timechannel_id())
# 	math_quat_y.set_unit_string("deg")
# 	math_quat_y.write("yaw")
