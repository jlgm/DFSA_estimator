import numpy as np
import matplotlib.pyplot as plt

lines = [line.rstrip('\n') for line in open('out.txt')]

axis = lines[0].split()

lb_slots = lines[1].split()
lb_time = lines[2].split()
lb_empty = lines[3].split()
lb_cols = lines[4].split()

el_slots = lines[5].split()
el_time = lines[6].split()
el_empty = lines[7].split()
el_cols = lines[8].split()

sc_slots = lines[9].split()
sc_time = lines[10].split()
sc_empty = lines[11].split()
sc_cols = lines[12].split()

# cmd = lines[13].split()[0]

##if not using Q, comment
q_slots = lines[13].split()
q_time = lines[14].split()
q_empty = lines[15].split()
q_cols = lines[16].split()

cmd = lines[17].split()[0]
##until here

if (cmd == "1"):
    plt.plot(axis, lb_slots, 'g^--', label="lower_bound")
    plt.plot(axis, el_slots, 'bs--', label="eom-lee")
    plt.plot(axis, sc_slots, 'ro--', label="schoute")
    plt.plot(axis, q_slots, 'mo-', label="Q")
elif (cmd == "2"):
    plt.plot(axis, lb_time, 'g^--', label="lower_bound")
    plt.plot(axis, el_time, 'bs--', label="eom-lee")
    plt.plot(axis, sc_time, 'ro--', label="schoute")
    plt.plot(axis, q_time, 'mo-', label="Q")

elif (cmd == "3"):
    plt.plot(axis, lb_empty, 'g^--', label="lower_bound")
    plt.plot(axis, el_empty, 'bs--', label="eom-lee")
    plt.plot(axis, sc_empty, 'ro--', label="schoute")
    plt.plot(axis, q_empty, 'mo-', label="Q")
elif (cmd == "4"):
    plt.plot(axis, lb_cols, 'g^--', label="lower_bound")
    plt.plot(axis, el_cols, 'bs--', label="eom-lee")
    plt.plot(axis, sc_cols, 'ro--', label="schoute")
    plt.plot(axis, q_cols, 'mo-', label="Q")

plt.legend(loc=2)
plt.grid(True)
plt.show()
