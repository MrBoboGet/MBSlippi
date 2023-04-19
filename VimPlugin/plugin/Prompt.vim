let s:PromptBufferWindow = -1
let s:GlobalJob = -1
let s:JobChannel = -1
let s:PromptBuffer = -1
let s:ResultBuffer = -1

let g:Response = ""
let g:MessageLength = -1
let g:Data = -1

function BigEndianParseInteger(Blob,Offset,IntSize) abort
    let ReturnValue = 0
    let i = 0
    while( i < a:IntSize)
        let ReturnValue += a:Blob[i+a:Offset] << ((a:IntSize-1-i)*8)
        let i += 1
    endwhile
    return(ReturnValue)
endfunction

function BigEndianEncodeInteger(IntToEncode,IntSize) abort
    let ReturnValue = 0z
    let i = 0
    while(i < a:IntSize)
        let ReturnValue[i] = list2blob([ and(a:IntToEncode >> ( (a:IntSize-1-i)*8 ),0xff)])[0]
        let i += 1
    endwhile
    return(ReturnValue)
endfunction

"function s:Reset()
"
"endfunction


function PromptCallback(Text) abort
    let MessageToSend = #{method: "execute",params: #{statement: a:Text}}
    let g:Response = s:SendMessage(json_encode(MessageToSend))
    echo g:Response
    echo s:ResultBuffer
    if (g:Response->has_key("error"))
        call appendbufline(s:ResultBuffer,"$",g:Response["error"])
    elseif(g:Response->has_key("result"))
        call appendbufline(s:ResultBuffer,"$",g:Response["result"])
    endif
endfunction

function s:ReadMessage(Channel) abort
    let g:Data = ch_readblob(a:Channel)
    echo g:Data
    if(len(g:Data) < 4)
        echo "Recieved to few bytes for s:ReadMessage, aborting"
        call s:Reset()
        return {}
    endif
    let g:MessageLength = BigEndianParseInteger(g:Data,0,4)
    while(ch_status(s:JobChannel) == "open" && len(g:Data) - 4 < g:MessageLength)
        let g:Data += ch_readblob(a:Channel)
    endwhile
    let MessageString =list2str(blob2list(g:Data[4:-1]))
    return json_decode(MessageString)
endfunction


function s:SendMessage(StringToSend) abort
    call ch_sendraw(s:JobChannel,BigEndianEncodeInteger(len(a:StringToSend),4))
    call ch_sendraw(s:JobChannel,a:StringToSend)
    "returns dictionary
    return s:ReadMessage(s:JobChannel)
endfunction

function StartPrompt() abort
    let s:GlobalJob = job_start("mbslippi server",#{in_mode: "raw",out_mode: "raw"})
    let s:JobChannel = job_getchannel(s:GlobalJob)
    let s:PromptBuffer = bufadd("")
    let s:ResultBuffer = bufadd("")
    vsplit 
    exec "normal \<c-w>l"
    split
    "in result part
    exec "buffer " .. s:ResultBuffer
    exec "normal \<c-w>j"
    exec "buffer " .. s:PromptBuffer
    set buftype=prompt
    set filetype=slpspec
    call prompt_setprompt(bufnr(),"")
    call prompt_setcallback(bufnr(),funcref("PromptCallback"))
endfunction
