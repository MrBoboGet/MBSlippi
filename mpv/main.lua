--function on_pause_change(name, value)
--    if value == true then
--        mp.set_property("fullscreen", "no")
--    end
--end
--mp.observe_property("pause", "bool", on_pause_change)

print("Loading mbslippi...")

ActiveGameInfo = {}
CurrentFrame = 0


function on_file_change(name,value)
    print("File was changed")
    if(not value) then
        return
    end

    JsonFile = string.gsub(value,"%.[^%.]+$",".json")
    print("JSON File: " .. JsonFile)
    if(file_exists(JsonFile)) then
        FileContents = io.open(JsonFile):read("a") 
        ActiveGameInfo = json.decode(FileContents)
    end 
    print(ActiveGameInfo.SituationBegin)
end

function SearchAbsoluteFrame(FrameIndex)
    mp.command("seek " .. FrameIndex/60 .. " exact+absolute ")
end


function GetCurrentFrameIndex()
    return mp.get_property_number("estimated-frame-number")
end

function NextSituation()
    CurrentFrame = GetCurrentFrameIndex()
    print("Current frame: " .. CurrentFrame)
    if( next(ActiveGameInfo) == nil) then
        return
    end
    print("Game info exists")
    for i,value in ipairs(ActiveGameInfo.SituationBegin) do
        if(value > CurrentFrame) then
            SearchAbsoluteFrame(value)
            break
        end
    end
end
function PreviousSituation()
    CurrentFrame = GetCurrentFrameIndex()
    print("Current frame: " .. CurrentFrame)
    if( next(ActiveGameInfo) == nil) then
        return
    end
    for i=#(ActiveGameInfo.SituationBegin),1,-1  do
        if(ActiveGameInfo.SituationBegin[i] < CurrentFrame) then
            SearchAbsoluteFrame(ActiveGameInfo.SituationBegin[i])
            break
        end
    end
end

mp.observe_property("path", "string", on_file_change)

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
mp.add_forced_key_binding("h","step",StepBack,{repeatable=true})
mp.add_forced_key_binding("l","step1",StepForward,{repeatable=true})
mp.add_forced_key_binding("H","step2",StepBigBack)
mp.add_forced_key_binding("L","step3",StepBigForward)
mp.add_forced_key_binding("j","step4",PreviousSituation)
mp.add_forced_key_binding("k","step5",NextSituation)
