require 'socket'
include Socket::Constants


socket = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(3333, 'localhost')
socket.bind(sockaddr)
socket.listen(5)
begin
  client_socket, client_sockaddr = socket.accept_nonblock
rescue Errno::EAGAIN, Errno::ECONNABORTED, Errno::EINTR, Errno::EWOULDBLOCK
  IO.select([socket])
  retry
end
puts client_socket.readline.chomp
client_socket.puts "hi from the server"
client_socket.close
socket.close