function(TurnOffWarnings)
    if(MSVC)
        add_compile_options(/W0)
    else()
        add_compile_options(-w)
    endif(MSVC)
endfunction(TurnOffWarnings)

function(TurnOnWarnings)
    if(MSVC)
        add_compile_options(/W3)
    else()
        add_compile_options(-Wall)
    endif(MSVC)
endfunction(TurnOnWarnings)
