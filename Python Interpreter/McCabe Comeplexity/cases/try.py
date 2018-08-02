def f():
  for x in range(3):
    if x < 3:
      print x
    else:
      print x+1


def getnum(val):
	if val > 3:
		try:
			val=int(raw_input("Enter a number "))
		except Exception, e:
			print "Error -", e
		if val < 1:
			try:
				val=raw_input("Renter a number ")
			except Exception, e:
				print "Error -", e
		else:
			print val

f(2)

getnum(4)



