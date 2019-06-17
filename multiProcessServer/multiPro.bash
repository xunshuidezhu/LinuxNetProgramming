func1 () {
    ./client
}
func2 () {
    ./client
}
func3 () {
    ./client
}

func1 &
func2 &
func3 &
wait