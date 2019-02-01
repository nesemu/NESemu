with open('nestest.log', 'r') as log:
	lines = log.readlines()

newlines = []
for line in lines:
	l = []
	l.append(line.split()[0])
	for i in line.split():
		if i[0:2] == 'P:' or i[0:2] == 'A:' or i[0:2] == 'X:' or i[0:2] == 'Y:' or i[0:3] == 'SP:':
			l.append(i)
	newlines.append(l)

final = [' '.join(line) for line in newlines]

with open('addressandp.log', 'w') as newlog:
	for line in final:
		newlog.write(line+'\n')