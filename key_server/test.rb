#!/usr/bin/ruby

require "osx/cocoa"
include OSX
OSX.require_framework 'ScriptingBridge'

iTunes = OSX::SBApplication.applicationWithBundleIdentifier_("com.apple.iTunes")

puts iTunes.currentTrack.name


# Hash[a.chars.zip(b.chars)].values.join == b.chars.uniq.join

def subjective(a,b)
  mapping = Hash[a.chars.zip(b.chars)]
  a.chars.map{|e|mapping[e]}.join == b
end

def iso(a,b)
  subjective(a, b) && subjective(b, a)
end