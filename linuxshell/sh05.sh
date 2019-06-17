# 1.input the file name && check if the filename is exist
echo -e "please input a filename, i will check the filename's type and \
permission. \n\n"
read -p "input a filename : " filename
test -z $filename && echo "you must input a filename." && exit 0
# 2.check the filename 
test ! -e $filename && echo "the ${filename} which you input isn't exist" || echo "${filename} is exist!" || exit 0
# 3.the input name is a file or a directory
test -f $filename && filetype="file" && echo -e "${filename} is a file!\ncongratulations! "
test -d $filename && echo "${filename} is a directory!  congratulations!"
# 4.echo the filetype
echo "the filename is:" ${filename} && echo "the filetype is:" ${filetype}

# # 5.read y/n
# read -p "please input (y/n): " input
# [ "$input" == "Y" -o "$input" == "y" ] && echo "OK, continue!" && exit 0
# [ "$input" == "N" -o "$input" == "n" ] && echo "!ERROR"  && exit 0
# echo "you need input y/n" && exit 0

# 6.read y/n -use if
read -p "please input (y/n)" input
if [ ${input} == "y" ] || [ ${input} == "Y" ]; then
    echo "OK, continue!"
#    exit 0
fi
if [ ${input} == "n" ] || [ ${input} == "N" ]; then
    echo "ERROR!"
#    exit 0
fi
if [ ${input} != "begin" ]; then    
    echo "what's it?"
#    exit 0
fi

# 7.check
read -p "input yourname : " name
read -p "input the password : " password
if [ ${name} == "xunshuidezhu" ] && [ ${password} == "abcd5698298" ]; then
    echo "welcome!"
#    exit 0
elif [ ${name} != "xunshuidezhu" ] ; then
    echo "the name error"
#    exit 0
else 
    echo "the pwd error"
#    exit 0
fi

# 8.check port
testing=$(netstat -tuln | grep "8090")
if [ $${testing} != " " ]; then
    echo "the server start success"
fi

# 9. function
function printit() {
    echo -n "your choice is: "
}
read i
case $i in
    "one")
        printit; echo $i ;;
    "exit")
        printit; echo "end" ;;
esac

function sum(){
 
 val1=$1
 
 val2=$2
 
 val3=$(($1+$2))
 
 return $val3
 
}
 
#Call function with 2 parameters and it return 1 parameter.
ret_val=${sum 10 20}
echo $ret_val

# function func() {
#     a=$1
#     b=$2
#     res=$((a+b))
#     echo res
# }

# res=$(1 2)
# echo $res