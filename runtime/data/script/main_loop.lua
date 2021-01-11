local graphics = require("graphics")
local ui = require("ui")

local function main_loop(initial_state)
   local app = __APP

   local ui_layers = {}

   local update_thread = coroutine.create(function()
      while true do
         for _, layer in ipairs(ui_layers) do
            graphics.reset_context()
            ui.clear()
            layer:update()
            ui.render()
         end
         coroutine.yield()
      end
   end)

   if initial_state.on_shown then
      initial_state:on_shown()
   end
   ui_layers[#ui_layers + 1] = initial_state
   while app:update() do
      local ok, result = coroutine.resume(update_thread)
      if not ok then
         error(result)
      end

      -- TODO
      if __APP:is_headless() then
         return "exit"
      end
   end
end

return main_loop
