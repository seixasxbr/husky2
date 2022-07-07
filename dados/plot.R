library(ggplot2)

setwd("/home/seixas/Documents/husky_ws/src/husky/bags/plot.R")

Xr <- read.csv("husky_2022-07-06-17-14-51-Xr.csv",header=T, sep = ",")
Yr <- read.csv("husky_2022-07-06-17-14-51-Yr.csv",header=T, sep = ",")
PSIr <- read.csv("husky_2022-07-06-17-14-51-PSIr.csv",header=T, sep = ",")
X <- read.csv("husky_2022-07-06-17-14-51-Xr.csv",header=T, sep = ",")
Y <- read.csv("husky_2022-07-06-17-14-51-Yr.csv",header=T, sep = ",")
PSI <- read.csv("husky_2022-07-06-17-14-51-PSI.csv",header=T, sep = ",")


gt.dados = data.frame(x=gt$.pose.pose.position.x,
                   y=gt$.pose.pose.position.y,
                   psi=PSI$.data)


gt.df = data.frame(x=gt$.pose.pose.position.x,
                y=gt$.pose.pose.position.y,
                psi=gt$.pose.pose.position.z)

odom.df = data.frame(x=odom$.pose.pose.position.x,
                   y=odom$.pose.pose.position.y,
                   z=odom$.pose.pose.position.z)

odomf.df = data.frame(x=odomf$.pose.pose.position.x,
                   y=odomf$.pose.pose.position.y,
                   z=odomf$.pose.pose.position.z)
colors <- c("Ground Truth" = "red", "Odometria" = "green", "Odometria Filtrada" = "blue")
library(ggplot2)
# Basic line plot with points
a = ggplot() +
  geom_point(data=odom.df, aes(x=x, y=y, group=1,color="Odometria")) +
  geom_point(data=odomf.df, aes(x=x, y=y, group=1,color="Odometria Filtrada")) +
  geom_point(data=gt.df, aes(x=x, y=y, group=1,color="Ground Truth"))
a = a + labs(title = "Odometrias x Ground Truth") + labs(color="Legenda") + xlab("X (m)") + ylab("Y (m)")
a
error.odom.x = gt.df$x - odom.df$x
error.odom.y = gt.df$y - odom.df$y
error.odom.z = gt.df$z - odom.df$z
error.odomf.x = gt.df$x - odomf.df$x
error.odomf.y = gt.df$y - odomf.df$y
error.odomf.z = gt.df$z - odomf.df$z


RMSE.odom.x = sqrt(sum(error.odom.x^2)/length(error.odom.x))
RMSE.odom.y = sqrt(sum(error.odom.y^2)/length(error.odom.y))
RMSE.odom.z = sqrt(sum(error.odom.z^2)/length(error.odom.z))
RMSE.odomf.x = sqrt(sum(error.odomf.x^2)/length(error.odomf.x))
RMSE.odomf.y = sqrt(sum(error.odomf.y^2)/length(error.odomf.y))
RMSE.odomf.z = sqrt(sum(error.odomf.z^2)/length(error.odomf.z))

tempo = gt$time
error.df = data.frame(odom.x = error.odom.x,
                      odom.y = error.odom.y,
                      odom.z = error.odom.z,
                      odomf.x = error.odomf.x,
                      odomf.y = error.odomf.y,
                      odomf.z = error.odomf.z)

b = ggplot() +
  geom_line(data = error.df,aes(x=(1:1671),y=odom.x, color = "Odometria"), size = 1.5) +
  geom_line(data = error.df,aes(x=(1:1671),y=odomf.x, color = "Odometria Filtrada"), size = 1.5) +
  labs(title = "Erro das Odometrias (X)") + labs(color="Legenda") + xlab("Tempo") + ylab("X (m)")
b

c = ggplot() +
  geom_line(data = error.df,aes(x=(1:1671),y=odom.y, color = "Odometria"), size = 1.5) +
  geom_line(data = error.df,aes(x=(1:1671),y=odomf.y, color = "Odometria Filtrada"), size = 1.5) +
  labs(title = "Erro das Odometrias (Y)") + labs(color="Legenda") + xlab("Tempo") + ylab("Y (m)")
c

d = ggplot() +
  geom_line(data = error.df,aes(x=(1:1671),y=odom.z, color = "Odometria"), size = 1.5) +
  geom_line(data = error.df,aes(x=(1:1671),y=odomf.z, color = "Odometria Filtrada"), size = 1.5) +
  labs(title = "Erro das Odometrias (Z)") + labs(color="Legenda") + xlab("Tempo") + ylab("Z (m)")
d
