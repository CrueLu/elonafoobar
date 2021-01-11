local Message = {}

local i18n = require("i18n")

--- @native
Message.txt_raw = _ENV.native.Message.txt_raw

function Message.txt(i18n_key, ...)
   Message.txt_raw(i18n.get(i18n_key, ...))
end

return Message
