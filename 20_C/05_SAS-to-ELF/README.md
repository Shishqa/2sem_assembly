## Лабораторная работа "Бинарный транслятор"

Цель работы: написать программу, переводящую бинарный код ![процессора](https://github.com/Shishqa/MIPT_1sem_C_lang/tree/master/Projects/03_CPU-Emulator), написанного в прошлом году, в бинарный код intel x86-64 команд. Также оттранслированный код оборачивается в интерфейс ELF-файла так, что его можно запустить в Linux 64-bit.

### Структура получаемого файла 

![image](https://user-images.githubusercontent.com/53887365/82755984-6d0dd380-9e01-11ea-9286-096ac0a323fc.png)

### Дополнительная оптимизация

При выполнении трансляции есть опция оптимизации последовательности команд.

Поддерживаемые оптимизации:
1. ```asm
        push  <something>
        pop   <somewhere>
      
      ; заменяется на
  
        mov   <somewhere>, <something>
   ```
  
2. ```asm
      ; убирается
        jmp   .label
      .label:

### Тест производительности

Вычисление 25-го числа Фибоначчи на старом ![эмуляторе](https://github.com/Shishqa/MIPT_1sem_C_lang/tree/master/Projects/03_CPU-Emulator)

![image](https://user-images.githubusercontent.com/53887365/82309364-3d1f9400-99ed-11ea-99d7-ca559cb0277f.png)

Вычисление 25-го числа Фибоначчи оттранслированной программой
![image](https://user-images.githubusercontent.com/53887365/82309694-a43d4880-99ed-11ea-9481-1f46fb792645.png)

Видно, что прирост производительности колоссальный из-за выполнения команд напрямую процессором.

В поисках потолка производительности для оттранслированной программы было проверено время её работы при вычислении больших чисел Фибоначчи:

Вычисление 45-го числа Фибоначчи оттранслированной программой
![image](https://user-images.githubusercontent.com/53887365/82308996-bec2f200-99ec-11ea-8c41-6e494f85118e.png)
