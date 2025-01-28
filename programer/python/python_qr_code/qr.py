import cv2
import logging
# import numpy as np
from pyzbar.pyzbar import decode

# from PIL import Image, ImageDraw, ImageFont

logger2 = logging.getLogger(__name__)
logger2.setLevel(logging.DEBUG)
# настройка обработчика и форматировщика для logger2
handler2 = logging.FileHandler(f"{__name__}.log", mode='a')
formatter2 = logging.Formatter("%(name)s %(asctime)s %(levelname)s %(message)s")
# добавление форматировщика к обработчику
handler2.setFormatter(formatter2)
# добавление обработчика к логгеру
logger2.addHandler(handler2)

def recognize_qr_code_from_camera():

    recognized_texts = ""
    # Запуск видеопотока с камеры
    cap = cv2.VideoCapture(0)  # Индекс 0 для стандартной камеры

    if not cap.isOpened():
        logger2.error("Не удалось открыть камеру.")
        print("Не удалось открыть камеру.")
        return recognized_texts

    # print("Ожидание QR-кода. Нажмите 'q' для выхода.")

    while True:
        ret, frame = cap.read()
        if not ret:
            logger2.error("Не удалось получить кадр с камеры.")
            # print("Не удалось получить кадр с камеры.")
            break

        # Распознаем QR-коды на текущем кадре
        decoded_objects = decode(frame)
        for obj in decoded_objects:
            # Вывод данных из QR-кода
            qr_data = obj.data.decode("utf-8")
            logger2.info(f"QR-код найден: {qr_data}")
            # print("QR-код найден:")
            # print(f"Данные: {qr_data}")
            # print(f"Тип: {obj.type}")
            recognized_texts = qr_data

        #     # Рисуем рамки вокруг QR-кодов
        #     points = obj.polygon
        #     if points:
        #         # Преобразуем координаты точек в целые числа
        #         pts = [(int(point.x), int(point.y)) for point in points]
        #         pts = np.array(pts, np.int32)  # Преобразуем в numpy массив
        #         pts = pts.reshape((-1, 1, 2))  # Изменяем форму массива
        #         cv2.polylines(frame, [pts], isClosed=True, color=(0, 255, 0), thickness=3)
        #         # Выводим данные QR-кода на изображении
        #         frame_pil = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
        #         draw = ImageDraw.Draw(frame_pil)
        #         font = ImageFont.truetype("arial.ttf", 20)
        #         draw.text((obj.rect.left, obj.rect.top - 30), obj.data.decode('utf-8'), font=font, fill=(0, 255, 0, 0))
        #         frame = cv2.cvtColor(np.array(frame_pil), cv2.COLOR_RGB2BGR)

        # # Отображаем текущий кадр
        # cv2.imshow("QR Code Scanner", frame)

        # Выход из цикла при нажатии клавиши 'q'
        # if cv2.waitKey(1) & 0xFF == ord("q"):
        # break

        if recognized_texts != "":
            break

    # Освобождение ресурсов
    cap.release()
    cv2.destroyAllWindows()
    return recognized_texts


