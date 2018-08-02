def f():
  x = 5
  print x
  def g():
    x = 55
    print x
    def h():
      x = 555
      print x
      def m():
        x= 5555
        print x
      m()
      print x
    h()
    print x
  g()
  print x


f()
