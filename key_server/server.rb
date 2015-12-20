require 'socket'
require 'appscript'

def osascript(script)
  system 'osascript', *script.split(/\n/).map { |line| ['-e', line] }.flatten
end

# q = Queue.new
# q.push(i)
# i = q.pop

puts "starting thread"
th = Thread.new do # Here we start a new thread
  Thread.current['speed'] = 0
  loop do
    speed = Thread.current['speed']
    speed -= 1
    speed = 0 if speed < 0
    Thread.current['speed'] = speed

    puts "speed is #{speed}" if speed != 0
    sleep 0.0000005

    # LEFT: (key code 123)
    # RIGHT: key code 124)
    # UP: (key code 126)
    # DOWN: (key code 125)
    if speed != 0
      # osascript <<-END
  # tell application "System Events"
  #   key code 123
  # end tell
  #     END
      Appscript.app("System Events").key_code(123)
    end
  end
  return nil
end

# puts "B"
# while th['counter'].to_i < 10 do
# # th is the long running thread and we can access the same variable as from inside the thread here
# # keep in mind that this is not a safe way of accessing thread variables, for reading status information
# # this works fine though. Read about Mutex to get a better understanding.
#   puts "Counter is #{th['speed']}"
#   sleep 0.5
# end

puts "Init script"
100.times do
  Appscript.app("System Events").key_code(124)
end
abort('done')

str =  <<-END
  tell application "System Events"
    key code 123
  end tell
END
# it = Appscript.app('System Events')
# it.run

# app("TextEdit").activate
# Appscript.app("System Events").keystroke("Look Ma, keystrokes!")
# Appscript.app("System Events").key_code(123)
# abort('end')


#########################################################################################################
#########################################################################################################

a = TCPServer.new('', 3333) # '' means to bind to "all interfaces", same as nil or '0.0.0.0'
loop {
  connection = a.accept

  puts "received:" + connection.recv(1024)
  # puts "received:" + connection.recv(1024)
  # connection.recv_nonblock(1024)
  puts "LOOP TICK #{a}"
  th['speed'] = 15

  if false
    osascript <<-END
  tell application "System Events"
    keystroke "w"
  end tell
    END
  end
  # using {control down, option down, command down}


  connection.write 'got something--closing now--here is your response message from the server'
  connection.close
}