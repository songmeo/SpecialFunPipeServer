# SpecialFunPipeServer
![gui](/gui.png)

This is a gui application using qt5, qtDesigner and qCustomPlot. 
The client sends x0, xn, number of points, order to server to compute.
Server sends back x,y values as double numbers or if compute fails, server sends error msgs. 
Client then put these numbers into Qvectors and use them as arguments for qCustomPlot.
