def f(x):
  if x <= 1:
    return 1
  else:
    return x*f(x-1)

print f(3)
