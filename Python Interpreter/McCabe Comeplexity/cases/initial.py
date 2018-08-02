
def f():
  for x in range(3):
    if x < 3:
      print x
    else:
      print x+1

def gh(n):
  x = 0
  while x < n:
    if x < 3:
      print x
    else:
      print x+1
    x += 1

def h():
  sum = 0
  for x in range(3):
    for y in range(4):
      for z in range(5):
        sum += z
  return sum

f()
gh(3)
h()
