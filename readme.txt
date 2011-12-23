How to use the code:
1. Type "make" in the terminal to compile the code using Makefile.
2. Execute the generated fpgaTest.exe to run the code.
3. Do not forget to put the following four input text files in the directory where you want to run the code.
netlist.txt
original_fault_list.txt
delay_values.txt
logic_simulator_input_sequence.txt

preprocessor output of test case 1:
Primary inputs: 1 2 
Fanout branches: 4 5 6 7 8 9 
Primary outputs: 10 
PPI: 3 
PPO: 11 
Number of nodes = 11
Number of primary inputs = 2
Number of fanout branches = 6
Number of primary outputs = 1
Number of PPI = 1
Number of PPO = 1
Number of level = 2
Clock period = 5
Flipflop initial value = 0

logic simulation output of test case 1:
line 1: (0 @ 0)(1 @ 5)(0 @ 10)(1 @ 15)(0 @ 20)(1 @ 25)(0 @ 30)(1 @ 35)(0 @ 40)(1 @ 45)
line 2: (1 @ 0)(0 @ 25)
line 3: (0 @ 0)(1 @ 10)(0 @ 35)
line 4: (0 @ 0)(1 @ 5)(0 @ 10)(1 @ 15)(0 @ 20)(1 @ 25)(0 @ 30)(1 @ 35)(0 @ 40)(1 @ 45)
line 5: (0 @ 0)(1 @ 5)(0 @ 10)(1 @ 15)(0 @ 20)(1 @ 25)(0 @ 30)(1 @ 35)(0 @ 40)(1 @ 45)
line 6: (1 @ 0)(0 @ 25)
line 7: (1 @ 0)(0 @ 25)
line 8: (0 @ 0)(1 @ 10)(0 @ 35)
line 9: (0 @ 0)(1 @ 10)(0 @ 35)
line 10: (1 @ 4)(0 @ 8)(1 @ 19)(0 @ 23)(1 @ 34)(0 @ 43)(1 @ 49)
line 11: (0 @ 4)(1 @ 8)(0 @ 34)

preprocessor output of test case 2:
Primary inputs: 1 2 7 8 9 10 14 17 18
Fanout branches: 5 6 12 13
Primary outputs: 16
PPI: 3
PPO: 19
Number of nodes = 19
Number of primary inputs = 9
Number of fanout branches = 4
Number of primary outputs = 1
Number of PPI = 1
Number of PPO = 1
Number of level = 5
Clock period = 15
Flipflop initial value = 1

logic simulation output of test case 2:
line 1: (0 @ 0)(1 @ 15)(0 @ 30)(1 @ 45)(0 @ 60)(1 @ 75)(0 @ 105)(1 @ 120)
line 2: (1 @ 0)(0 @ 15)(1 @ 30)(0 @ 60)(1 @ 90)(0 @ 105)(1 @ 120)(0 @ 135)
line 3: (1 @ 0)(0 @ 30)(1 @ 45)(0 @ 60)(1 @ 75)(0 @ 120)
line 4: (1 @ 2)(0 @ 16)(1 @ 107)(0 @ 121)
line 5: (1 @ 2)(0 @ 16)(1 @ 107)(0 @ 121)
line 6: (1 @ 2)(0 @ 16)(1 @ 107)(0 @ 121)
line 7: (0 @ 0)(1 @ 30)(0 @ 60)(1 @ 105)(0 @ 120)
line 8: (1 @ 0)(0 @ 45)(1 @ 60)(0 @ 75)(1 @ 90)(0 @ 105)(1 @ 135)
line 9: (1 @ 0)(0 @ 90)(1 @ 105)(0 @ 135)
line 10: (1 @ 0)(0 @ 15)(1 @ 45)(0 @ 60)(1 @ 75)(0 @ 105)(1 @ 120)
line 11: (0 @ 3)(1 @ 33)(0 @ 46)(1 @ 110)(0 @ 122)
line 12: (0 @ 3)(1 @ 33)(0 @ 46)(1 @ 110)(0 @ 122)
line 13: (0 @ 3)(1 @ 33)(0 @ 46)(1 @ 110)(0 @ 122)
line 14: (0 @ 0)(1 @ 15)(0 @ 30)(1 @ 90)
line 15: (0 @ 4)(1 @ 92)(0 @ 109)(1 @ 137)
line 16: (0 @ 4)
line 17: (0 @ 0)(1 @ 30)(0 @ 45)(1 @ 75)(0 @ 105)(1 @ 120)
line 18: (1 @ 0)(0 @ 15)(1 @ 30)(0 @ 45)(1 @ 60)(0 @ 75)(1 @ 105)(0 @ 135)
line 19: (1 @ 4)(0 @ 17)(1 @ 34)(0 @ 47)(1 @ 61)(0 @ 111)(1 @ 136)

preprocessor output of test case 3:
Primary inputs: 10 20 30 11 21 12 22 13 23 14 24 15 25 16 26 17 27 18 28 
Fanout branches: 40 50 60 70 80 90 41 51 61 71 81 91 42 52 62 72 82 92 43 53 63 73 83 93 44 54 64 74 84 94 45 55 65 75 85 95 46 56 66 76 86 96 47 57 67 77 87 97 48 58 68 78 88 98 
Primary outputs: 2 5 118 8 
PPI: 33 36 
PPO: 112 115 
Number of nodes = 96
Number of primary inputs = 19
Number of fanout branches = 54
Number of primary outputs = 4
Number of PPI = 2
Number of PPO = 2
Max number of level = 7
Clock period = 13
Flipflop initial value = 0 0 

logic simulation output of test case 3:
line 2: (1 @ 5)(0 @ 6)(1 @ 16)(0 @ 23)(1 @ 34)
line 5: (1 @ 5)(0 @ 17)(1 @ 27)
line 8: (1 @ 5)(0 @ 30)(1 @ 40)
line 10: (0 @ 0)(1 @ 13)(0 @ 26)
line 11: (0 @ 0)(1 @ 13)(0 @ 26)
line 12: (0 @ 0)(1 @ 13)
line 13: (0 @ 0)(1 @ 13)
line 14: (0 @ 0)(1 @ 13)
line 15: (0 @ 0)(1 @ 13)
line 16: (0 @ 0)
line 17: (0 @ 0)(1 @ 26)
line 18: (0 @ 0)
line 20: (0 @ 0)(1 @ 13)(0 @ 26)
line 21: (0 @ 0)
line 22: (0 @ 0)(1 @ 26)
line 23: (0 @ 0)(1 @ 13)
line 24: (0 @ 0)
line 25: (0 @ 0)(1 @ 13)
line 26: (0 @ 0)(1 @ 26)
line 27: (0 @ 0)
line 28: (0 @ 0)(1 @ 13)(0 @ 26)
line 30: (0 @ 0)
line 31: (0 @ 4)(1 @ 17)(0 @ 30)
line 32: (0 @ 2)(1 @ 19)(0 @ 28)
line 33: (0 @ 0)(1 @ 26)
line 34: (0 @ 4)(1 @ 17)
line 35: (0 @ 2)(1 @ 19)
line 36: (0 @ 0)(1 @ 26)
line 37: (0 @ 4)(1 @ 30)
line 38: (0 @ 2)(1 @ 32)
line 40: (0 @ 0)(1 @ 13)(0 @ 26)
line 41: (0 @ 0)(1 @ 13)(0 @ 26)
line 42: (0 @ 0)(1 @ 13)
line 43: (0 @ 0)(1 @ 13)
line 44: (0 @ 0)(1 @ 13)
line 45: (0 @ 0)(1 @ 13)
line 46: (0 @ 0)
line 47: (0 @ 0)(1 @ 26)
line 48: (0 @ 0)
line 50: (0 @ 0)(1 @ 13)(0 @ 26)
line 51: (0 @ 0)(1 @ 13)(0 @ 26)
line 52: (0 @ 0)(1 @ 13)
line 53: (0 @ 0)(1 @ 13)
line 54: (0 @ 0)(1 @ 13)
line 55: (0 @ 0)(1 @ 13)
line 56: (0 @ 0)
line 57: (0 @ 0)(1 @ 26)
line 58: (0 @ 0)
line 60: (0 @ 0)(1 @ 13)(0 @ 26)
line 61: (0 @ 0)
line 62: (0 @ 0)(1 @ 26)
line 63: (0 @ 0)(1 @ 13)
line 64: (0 @ 0)
line 65: (0 @ 0)(1 @ 13)
line 66: (0 @ 0)(1 @ 26)
line 67: (0 @ 0)
line 68: (0 @ 0)(1 @ 13)(0 @ 26)
line 70: (0 @ 0)(1 @ 13)(0 @ 26)
line 71: (0 @ 0)
line 72: (0 @ 0)(1 @ 26)
line 73: (0 @ 0)(1 @ 13)
line 74: (0 @ 0)
line 75: (0 @ 0)(1 @ 13)
line 76: (0 @ 0)(1 @ 26)
line 77: (0 @ 0)
line 78: (0 @ 0)(1 @ 13)(0 @ 26)
line 80: (0 @ 0)
line 81: (0 @ 4)(1 @ 17)(0 @ 30)
line 82: (0 @ 2)(1 @ 19)(0 @ 28)
line 83: (0 @ 0)(1 @ 26)
line 84: (0 @ 4)(1 @ 17)
line 85: (0 @ 2)(1 @ 19)
line 86: (0 @ 0)(1 @ 26)
line 87: (0 @ 4)(1 @ 30)
line 88: (0 @ 2)(1 @ 32)
line 90: (0 @ 0)
line 91: (0 @ 4)(1 @ 17)(0 @ 30)
line 92: (0 @ 2)(1 @ 19)(0 @ 28)
line 93: (0 @ 0)(1 @ 26)
line 94: (0 @ 4)(1 @ 17)
line 95: (0 @ 2)(1 @ 19)
line 96: (0 @ 0)(1 @ 26)
line 97: (0 @ 4)(1 @ 30)
line 98: (0 @ 2)(1 @ 32)
line 100: (0 @ 2)
line 101: (1 @ 2)(0 @ 5)(1 @ 15)(0 @ 18)(1 @ 28)(0 @ 31)
line 102: (0 @ 3)(1 @ 5)(0 @ 20)(1 @ 31)
line 103: (0 @ 2)(1 @ 28)
line 104: (1 @ 2)(0 @ 5)(1 @ 15)(0 @ 18)
line 105: (0 @ 3)(1 @ 5)(0 @ 14)(1 @ 24)
line 106: (0 @ 2)
line 107: (1 @ 2)(0 @ 5)(1 @ 28)(0 @ 31)
line 108: (0 @ 3)(1 @ 5)(0 @ 27)(1 @ 37)
line 112: (0 @ 6)(1 @ 25)
line 115: (0 @ 6)(1 @ 19)
line 118: (0 @ 6)

