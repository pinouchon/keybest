task update_private: :environment do
  #
  `wget http://localhost:3001/privatexml`
  `mv privatexml /Users/pinouchon/Library/Application\\ Support/Karabiner/private.xml`
  # puts source
  # content = HTTParty.get('http://localhost:3001/privatexml')
  # File.open('/Users/pinouchon/Library/Application Support/Karabiner/private.xml', 'w') do |file|
  #   file.write(content)
  # end
end

