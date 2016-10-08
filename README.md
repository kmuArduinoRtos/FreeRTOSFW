# Getting started
KMU Arduino RTOS는 아두이노 환경에서 쉽게 멀티태스킹 프로그래밍을 할 수 있도록 도와주는 미들웨어 프레임워크입니다. [FreeRTOS](http://www.freertos.org/)는 이미 멀티태스킹을 위한 API를 지원하고 있지만, KMU Arduino RTOS는 FreeRTOS를 사용하기 어려운, 또는 이를 사용하기에는 너무 과한 초보 사용자들도 쉽게 아두이노 멀티태스킹 프로그래밍을 할 수 있도록 간편화하였습니다. KMU Arduino RTOS의 최신 버전은 항상 [이 곳](https://github.com/kmuArduinoRtos/FreeRTOSFW)에 공개되어 있습니다.

# Programing Process
KMU Arduion RTOS를 이용한 프로그래밍은 다음과 같은 순서로 진행됩니다:
1. 함수 정의
2. 태스크 정의
3. 스케줄러 실행

## 1. 함수 정의
"함수 정의"는 사용자가 직접 아두이노 프로그램 코드를 작성하는 부분입니다. 기존의 FreeRTOS와는 다르게 KMU Arduion RTOS에서는 초보자에게 불필요한 부분을 없애 다음과 같이 간단하게 함수를 정의할 수 있습니다.
```
void Thread1(){
...
}
```

## 2. 태스크 정의
"태스크 정의"는 위에서 사용자가 정의한 함수를 태스크로 만듭니다. 이 과정에서 사용자가 정의한 함수는 FreeRTOS계층에서 사용하는 "태스크"로 변환되어 스케줄의 대상이 됩니다. 또한 하나의 함수는 여러개의 태스크가 될 수 있습니다. 
```
fTaskDef(Thread1, 1000);
```
첫번째 파라미터는 위에서 정의한 사용자 함수의 이름입니다. 태스크가 될 대상이 되는 함수를 지정합니다.
두번째 파라미터는 태스크의 주기입니다. FreeRTOS의 태스크의 주기는 사용자가 정의한 함수의 총 실행시간이었지만, KMU Arduino RTOS에서는 태스크의 주기를 직접 명시해야합니다. 이 때 주기는 함수의 총 실행시간보다 커야합니다.

## 3. 스케줄러 실행
"스케줄러 실행"은 위에서 정의한 태스크들을 실행하는 함수입니다. FreeRTOS에서는 각 태스크들의 우선순위를 지정할 수 있으며, 우선순위가 높은 태스크가 우선적으로 실행됩니다. 반면, KMU Arduion RTOS는 각 태스크들의 우선순위가 "태스크 정의"에서 명시한 주기를 기준으로 자동적으로 부여됩니다. FreeRTOS에서는 짧은 주기를 가진 태스크는 상대적으로 높은 우선순위를 갖게 됩니다. 다시말해, 가장 짧은 주기의 태스크는 다른 태스크들보다 가장 먼저 실행됩니다.
```
fInitTasks();
```


# Example
아래는 위에서 소개한 프로그래밍 과정 세 단계의 이해를 돕기 위한 프로그래밍 예시입니다. 아래에서 설명하는 예제 프로그램은 [Arduino1.7.11](http://www.arduino.org/)을 기준으로 작성되었으며 [이곳](https://github.com/kmuArduinoRtos/FreeRTOSFW)에 공개되어 있습니다.

## Print words
두 개의 문자 A,B를 각각의 주기마다 시리얼모니터를 통해 출력하는 프로그램입니다.

### 1. 함수 정의
A와 B를 각각 출력하는 함수 두 개를 정의합니다.
```
void print_A() {
    Serial.print("A");
}

void print_B() {
    Serial.print("B");
}
```

### 2. 태스크 정의
위에서 정의한 print_A()와 print_B()를 태스크로 정의합니다. 
A는 1초마다, B는 3초마다 출력하도록 합니다.
```
fTaskDef(print_A, 1000);
fTaskDef(print_B, 3000);
```

### 3. 스케줄러 실행
스케줄러를 실행합니다.
```
fInitTasks();
```

### 실행 결과
```
ABAAABAAABAAAB...
```
스케줄러를 실행함과 동시에, A와 B가 출력되게 됩니다. 그런데, A의 주기는 1초이고 B의 주기는 3초이기 때문에, print_A의 태스크는 print_B보다 상대적으로 높은 우선순위를 갖게 됩니다. 그렇기 때문에, A가 B보다 먼저 실행되게 됩니다.

## RGB & 7Segment
RGB LED와 7Segment display를 동시에 실행하는 프로그램입니다.

### 1. 함수 정의
RGB LED를 점멸하는 함수와 7Segment를 표시하는 함수를 정의합니다.
RGB()는 1초마다 빨강, 초록, 파랑 순서대로 점멸하는 함수이며,
7segment()는 1초마다 1부터 10까지 차례대로 표시하는 함수입니다.
```
void RGB() {
   analogWrite(R,255); analogWrite(G,0); analogWrite(B,0);
   fDelay(1000);
   analogWrite(R,0); analogWrite(G,255); analogWrite(B,0);
   fDelay(1000);
   analogWrite(R,0); analogWrite(G,0); analogWrite(B,255);
}

void 7segment() {
  int i = 0;
  while (1) {
    for(i=1; i<10; i++){
      show(4,i); 
      fDelay(1000);
    }
  }
}
```
> 두 함수의 서브 함수는 [예제](https://github.com/kmuArduinoRtos/FreeRTOSFW)에서 확인하실 수 있습니다.

### 2. 태스크 정의
위에서 정의한 RGB()와 7segment()를 태스크로 정의합니다. 
RGB()는 3초마다, 7segment()는 10초마다 실행되도록 합니다.
```
fTaskDef(RGB, 3000);
fTaskDef(7segment, 10000);
```

### 3. 스케줄러 실행
스케줄러를 실행합니다.
```
fInitTasks();
```

### 실행 결과
```
...?
```


## ServoMotor & 8x8 dot matrix
...

### 1. 함수 정의
...
```
void RGB() {
   ...
}

void 7segment() {
  ...
}
```

### 2. 태스크 정의
...
```
fTaskDef(RGB, 3000);
fTaskDef(7segment, 10000);
```

### 3. 스케줄러 실행
스케줄러를 실행합니다.
```
fInitTasks();
```

### 실행 결과
```
...?
```
