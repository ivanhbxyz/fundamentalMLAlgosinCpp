#
#
# Name: ivanhb
# Naive Bayes Regression R script
#
library(e1071)
# DATA WRANGLING
df <- read.csv("titanic_project.csv", header = TRUE)

df <- df[,c(2,3,4,5)]
df$pclass <- factor(df$pclass)
df$survived <- factor(df$survived)
df$sex <- factor(df$sex)
train <- df[1:900,]
test <- df[(nrow(train)+1):nrow(df),]

startTime <- proc.time()
nb1 <- naiveBayes(survived~., data=train)

pred1 <- predict(nb1, newdata=test, type="class")
tb <- table(pred1, test$survived)

accuracy <- ( tb[1,1] + tb[2,2] )/( tb[1,1] + tb[1,2] + tb[2,1] + tb[2,2])
sensitivity <- ( tb[2,2] )/( tb[2,2] + tb[1,2])
specificity <- ( tb[1,1] )/( tb[2,1] + tb[1,1])

pred_raw <- predict(nb1, newdata=test, type="raw")
endTime <- proc.time() - startTime
print("SURVIVED?: ")
pred_raw [1:nrow(pred_raw),] # print out probabilities
print(nb1)

print(paste("Accuracy:: ", accuracy))
print(paste("Sensitivity:: ", sensitivity))
print(paste("Specificity:: ", specificity))

print("Modeling Time:: ")
print(endTime)
#


