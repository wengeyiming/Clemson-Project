def g():
  x = 0
  if x >= 0: 
    print 10
    x = 2
    if x: 
      print 1
      return
    else: 
      print 2
    print 100

g()
print 1000
