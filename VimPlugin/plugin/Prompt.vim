let s:PromptBufferWindow = -1
let s:GlobalJob = -1
let s:JobChannel = -1
let s:PromptBuffer = -1
let s:ResultBuffer = -1
let s:PromptWindow = -1
let s:ResultWindow = -1

let s:Executing = v:false
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

function s:Reset()
    call job_stop(s:GlobalJob)
    exec "bdelete! " .. s:PromptBuffer .. " " .. s:ResultBuffer
endfunction

function NotificationCallback(Channel,Notification) abort
    if(a:Notification.method == "execute/result")
        "Replace last line with result, or more if printing
        call deletebufline(s:ResultBuffer,"$")
        let Lines = split(a:Notification.params.result,"\n")
        for Line in Lines
            call appendbufline(s:ResultBuffer,"$","#" .. Line)
        endfor
        let s:Executing = v:false
    endif
endfunction

function! PromptCallback(Text) abort
    if(s:Executing)
        echo "Server is busy with executing previous statement"
    endif
    let MessageToSend = #{method: "execute",params: #{statement: a:Text}}
    let Response = ch_evalexpr(s:JobChannel,MessageToSend)
    if (Response->has_key("error"))
        call appendbufline(s:ResultBuffer,"$","#" .. Response["error"].message)
        call deletebufline(s:PromptBuffer,line("$",s:PromptWindow)-1)
    elseif(Response->has_key("result"))
        let s:Executing = v:true
        call appendbufline(s:ResultBuffer,"$",a:Text)
        if(Response["result"] != "")
            call appendbufline(s:ResultBuffer,"$","#" .. Response["result"])
        endif
        call deletebufline(s:PromptBuffer,line("$",s:PromptWindow)-1)
    endif
    "exec "normal \<c-w>k"
    "w
    "exec "normal \<c-w>j"
endfunction

function s:SendNotification(ObjectToSend)
    call ch_sendexpr(ObjectToSend)
endfunction

function s:SendMessage(StringToSend) abort
    return ch_evalexpr(s:JobChannel,StringToSend)
endfunction

function! StartPrompt() abort
    let s:GlobalJob = job_start("mbslippi server",#{in_mode: "lsp",out_mode: "lsp",callback: function("NotificationCallback")})
    let s:JobChannel = job_getchannel(s:GlobalJob)
    let s:PromptBuffer = bufadd("")
    let s:ResultBuffer = bufadd("asdasdasdasdasdassd")
    vsplit 
    exec "normal \<c-w>l"
    split
    "in result part
    exec "buffer " .. s:ResultBuffer
    "set filetype=slpspec
    "set buflisted
    e Temp.slpspec
    let s:ResultBuffer = bufnr()
    au WinClosed <buffer> call <SID>Reset()
    exec "normal \<c-w>j"
    exec "buffer " .. s:PromptBuffer
    "let b:coc_force_attach = 1
    set buftype=prompt
    au WinClosed <buffer> call <SID>Reset()
    call prompt_setprompt(bufnr(),"")
    call prompt_setcallback(bufnr(),funcref("PromptCallback"))
    let s:PromptWindow = bufwinid(s:PromptBuffer)
    let s:ResultWindow = bufwinid(s:ResultBuffer)
endfunction
