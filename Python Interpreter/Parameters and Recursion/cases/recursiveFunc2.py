def f(x,y):
	if x==1:
		return 1
	else:
		return x*y*f(x-1,y-1)


print f(3,3)


