## Alpha-blending 

Программа накладывает одну картинку в формате bmp 32-bit на другую такого же размера.

### Сборка и запуск 

```bash
  make 
  cd bin
  
  ./blend ../examples/CAT.bmp ../examples/DESK.bmp -o RES.bmp
  feh RES.bmp
```

### Пример работы программы

<details>
<summary>Передний план</summary>
<img src="https://github.com/Shishqa/MIPT_2sem_Assembly/blob/dev/20_C/04_Alpha-blending/example/CAT.bmp" width="500">
</details>


<details>
<summary>Задний план</summary>
<img src="https://github.com/Shishqa/MIPT_2sem_Assembly/blob/dev/20_C/04_Alpha-blending/example/DESK.bmp" width="500">
</details>

<details>
<summary>Результат работы программы</summary>
<img src="https://github.com/Shishqa/MIPT_2sem_Assembly/blob/dev/20_C/04_Alpha-blending/example/RES.bmp" width="500">
</details>

### Варианты работы программы

1. Производительный с использованием SSE-интринсиков.
2. Наивная реализация без использования преимуществ аппаратуры.

### Тест производительности

![image](https://user-images.githubusercontent.com/53887365/82736275-9ae80f00-9d52-11ea-9ae4-1a68ed8f23c2.png)

| Без SSE   | С SSE    |
| --------- | -------- |
| 23.17 сек | 7.44 сек |

Коэффициент ускорения: `3.11`


