--function on_pause_change(name, value)
--    if value == true then
--        mp.set_property("fullscreen", "no")
--    end
--end
--mp.observe_property("pause", "bool", on_pause_change)

ActiveGameInfo = {}

function on_file_change(name,value)
    local JsonFile = string.gsub(value,"%.[^%.]+$",".json")
    if(file_exists(JsonFile)) then
        local FileContents = io.open(JsonFile):read("a") 
        ActiveGameInfo = json.decode(FileContents)
    end 
end

function SearchAbsoluteFrame(FrameIndex)
    mp.command("seek " .. FrameIndex/60 .. " exact+absolute ")
end


function GetCurrentFrameIndex()
    return mp.get_property_number("estimated-framec-count")
end

function NextSituation()
    if( next(ActiveGameInfo) = nil) then
        return
    end
    local CurrentFrame = GetCurrentFrameIndex()
    for i,value in ipairs(ActiveGameInfo.SitautionBegin) do
        if(value > CurrentFrame) then
            SearchAbsoluteFrame(value)
            break
        end
    end
end
function PreviousSituation()
    if( next(ActiveGameInfo) = nil) then
        return
    end
    local CurrentFrame = GetCurrentFrameIndex()
    for i,value in ipairs(ActiveGameInfo.SitautionBegin) do
        if(value < CurrentFrame) then
            SearchAbsoluteFrame(value)
            break
        end
    end
end

mp.observe_property("path", "string", on_pause_change)

json = require "json"

function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

function StepBack()
    mp.command("frame-back-step")
end
function StepForward()
    mp.command("frame-step")
end
function StepBigForward()
    mp.command("seek " .. 20/60 .. " exact+relative ")
end
function StepBigBack()
    mp.command("seek -" .. 20/60 .. " exact+relative ")
end
mp.add_key_binding("h","step",StepBack,{repeatable=true})
mp.add_key_binding("l","step1",StepForward,{repeatable=true})
mp.add_key_binding("H","step2",StepBigBack)
mp.add_key_binding("L","step3",StepBigForward)
mp.add_key_binding("j","step3",PreviousSituation)
mp.add_key_binding("k","step3",NextSituation)
