set term gif animate delay 0 size 400, 400
set output "lorenz.gif"
do for [n=1:10000] { splot [-100:100][-100:100][-100:100] "data.txt" u 2:3:4 every ::::n w lp t sprintf("n=%i", n) }
