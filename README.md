# 6.506-Final-Project
cmd for khop with real life data
```
$ make khop
$ ./khop inputs/<graph_name>/graph
```

## translate graph into aspen format
```
$ make translate
$ ./translate inputs/<graph_name>/graph
```
output stored in `inputs/<graph_name>/graph.aspen.txt`

## running aspen
```
$ git clone https://github.com/ldhulipala/aspen.git
$ cd aspen/code
```
and then:
```
$ make run_static_algorithm
$ ./run_static_algorithm -t KHOP -f inputs/<graph_name>/graph.aspen.txt
```
