
-- hs.hotkey.bind({"cmd", "alt", "ctrl"}, "W", function()
--  hs.notify.new({title="Hammerspoon", informativeText="Hello World"}):send()
-- end)

hs.window.animationDuration = 0

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "A", function()
  hs.application.launchOrFocus("Finder")
end)

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "F", function() -- N
  hs.application.launchOrFocus("Notes")
end)

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "D", function() -- T
  hs.application.launchOrFocus("iTerm 2")
end)

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "S", function() -- S
  hs.application.launchOrFocus("Slack")
end)

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "G", function() -- R
  hs.application.launchOrFocus("RubyMine")
end)

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "C", function() -- C
  hs.application.launchOrFocus("Google Chrome")
end)

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "H", function()
  local win = hs.window.focusedWindow()
  local f = win:frame()

  f.x = f.x - 10
  win:setFrame(f)
end)

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "Left", function()
  local win = hs.window.focusedWindow()
  local f = win:frame()
  local screen = win:screen()
  local max = screen:frame()

  f.x = max.x
  f.y = max.y
  f.w = max.w / 2
  f.h = max.h
  win:setFrame(f)
end)

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "Right", function()
  local win = hs.window.focusedWindow()
  local f = win:frame()
  local screen = win:screen()
  local max = screen:frame()

  f.x = max.x + (max.w / 2)
  f.y = max.y
  f.w = max.w / 2
  f.h = max.h
  win:setFrame(f)
end)

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "R", function()
  hs.reload()
end)
hs.alert.show("Config loaded")

-- hs.hotkey.bind({"cmd", "alt", "ctrl"}, "A", function()
--   hs.alert.show("Hello World!")
-- end)

function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. ": "
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    elseif type(v) == 'boolean' then
      print(formatting .. tostring(v))
    else
      print(formatting .. v)
    end
  end
end

hs.hotkey.bind({"cmd", "alt", "ctrl"}, "N", function()
  -- tprint(hs.mouse.getAbsolutePosition())
  -- hs.eventtap.leftClick({630.0, 314.0})
  local ptMouse = hs.mouse.getAbsolutePosition()
  local types = hs.eventtap.event.types
  hs.eventtap.event.newMouseEvent(types.leftMouseDown, ptMouse, keys):post()
  hs.eventtap.event.newMouseEvent(types.leftMouseUp, ptMouse, keys):post()
end)