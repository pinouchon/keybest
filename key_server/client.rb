require 'socket'
a = TCPSocket.new('127.0.0.1', 3333) # could replace 127.0.0.1 with your "real" IP if desired.
a.write "hi server!"
puts "got back:" + a.recv(1024)
a.close