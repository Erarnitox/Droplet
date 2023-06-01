
# print a cmake variables to the command line
function(print var)
    message("${var} = ${${var}}")
endfunction()