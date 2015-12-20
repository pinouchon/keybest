require 'socket'
include Socket::Constants

socket = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(3333, 'localhost')
begin
  socket.connect_nonblock(sockaddr)
rescue Errno::EINPROGRESS
  IO.select(nil, [socket])
  begin
    socket.connect_nonblock(sockaddr)
  rescue Errno::EINVAL
    retry
  rescue Errno::EISCONN
  end
end
socket.write("hi from the client\n")
results = socket.read
puts results
socket.close