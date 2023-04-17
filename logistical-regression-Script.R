# Name, id: ivanhb
# Logistic Regression R script

library(HSAUR)
#LOADING DATA + WRANGLING
# Uncomment the below line to specify the correct path
df <- read.csv(".titanic_project.csv", header = TRUE)

train <- df[1:900,]
test <- df[901:1046,]

startTime <- proc.time()
#MODELING
glm1 <- glm(survived~pclass, data=train, family=binomial)
probs <- predict(glm1,  newdata=test, type="response")

pred <- ifelse(probs> 0.5, 1, 0)

tb <- table(pred, test$survived)

accuracy <- ( tb[1,1] + tb[2,2] )/( tb[1,1] + tb[1,2] + tb[2,1] + tb[2,2])
sensitivity <- ( tb[2,2] )/( tb[2,2] + tb[1,2])
specificity <- ( tb[1,1] )/( tb[2,1] + tb[1,1])

endTime <- proc.time() - startTime
#START PRINTOUT
print(summary(glm1))
print(paste("Accuracy:: ", accuracy))
print(paste("Sensitivity:: ", sensitivity))
print(paste("Specificity:: ", specificity))
print("Modeling Time::")
print(endTime)