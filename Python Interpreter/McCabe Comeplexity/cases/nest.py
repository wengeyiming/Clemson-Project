

def f(): 
	for x in range(4):
		if x < 2:
			print x
		else:
			print x+1
	def g(n):
		f()
		x = 0
		while x < n:
			x = x+1
		def g2():
			for x in range(4):
				if x < 2:
					print x

def h():
    sum = 0
    for x in range(4):
        for y in range(4):
            for z in range(4):
                sum +=z
    return sum


f()

h()
