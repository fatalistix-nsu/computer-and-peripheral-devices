#include <libusb-1.0/libusb.h>
#include <stdio.h>

void printdev(libusb_device *dev);

int main()
{
    libusb_device **devs;       // указатель на указатель на устройство,
                                // используется для получения списка устройств

    libusb_context *ctx = NULL; // контекст сессии libusb
    int r;                      // для возвращаемых значений
    ssize_t cnt;                // число найденных USB-устройств
    ssize_t i;                  // индексная переменная цикла перебора всех устройств

    // инициализировать библиотеку libusb, открыть сессию работы с libusb
    r = libusb_init(&ctx);
    if (r < 0)
    {
        fprintf(stderr,
                "Ошибка: инициализация не выполнена, код: %d.\n", r);
        return 1;
    }

    // задать уровень подробности отладочных сообщений
    //libusb_set_debug(ctx, 3);
    libusb_set_option(ctx, LIBUSB_OPTION_MAX);

    // получить список всех найденных USB- устройств
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0)
    {
        fprintf(stderr,
                "Ошибка: список USB устройств не получен.\n");
        return 1;
    }

    printf("найдено устройств: %ld\n", cnt);
    printf("================================="
           "================================\n");
    printf("* количество возможных конфигураций\n");
    printf("|  * класс устройства\n");
    printf("|  |    * идентификатор производителя\n");
    printf("|  |    |      * идентификатор устройства\n");
    printf("|  |    |      |      * количество интерфейсов\n");
    printf("|  |    |      |      |   * количество "
           "альтернативных настроек\n");
    printf("|  |    |      |      |   |  * класс устройства\n");
    printf("|  |    |      |      |   |  |   * номер интерфейса\n");
    printf("|  |    |      |      |   |  |   |  * количество "
           "конечных точек\n");
    printf("|  |    |      |      |   |  |   |  |  * тип дескриптора\n");
    printf("|  |    |      |      |   |  |   |  |  |  * адрес "
           "конечной точки\n");
    printf("+--+----+------+------+---+--+---+--+"
           "--+--+----------------------\n");

    for(i = 0; i < cnt; i++)        // цикл перебора всех устройств
    {
        printdev(devs[i]);      // печать параметров устройства
    }
    printf("================================="
           "================================\n");

    // освободить память, выделенную функцией получения списка устройств
    libusb_free_device_list(devs, 1);

    libusb_exit(ctx);   // завершить работу с библиотекой libusb,
                        // закрыть сессию работы с libusb
    return 0;
}

void printdev(libusb_device *dev)
{
    libusb_device_descriptor desc;                  // дескриптор устройства
    libusb_config_descriptor *config;               // дескриптор конфигурации объекта
    const libusb_interface *inter;                  // Интерфейс
    const libusb_interface_descriptor *interdesc;   // Дескриптор интерфейса
    const libusb_endpoint_descriptor *epdesc;       // Дескриптор конечной точки

    // Строки для вывода основных данных
    unsigned char uSerialNumber[256];
    unsigned char uManufacturer[256];
    unsigned char uProduct[256];

    libusb_device_handle* devicehandle;

    // Получает дескриптор устройства
    {
        int error = libusb_get_device_descriptor(dev, &desc); // Получает дескриптор устройства
        if (error < 0)  // Если ошибка
        {
            fprintf(stderr,
                    "Ошибка: дескриптор устройства не получен, код: %d.\n", error);
            return;
        }
    }

    // Позволяет получить доп инфо об устройстве
    {
        int error = libusb_open(dev, &devicehandle);
        if (error < 0)
        {
            printf("Ошибка: дескриптор устройства не получен, код: %d.\n", error);
            return;
        }
    }

    // Получает информацию о серийном номере устройства
    {
        int error =
                libusb_get_string_descriptor_ascii(devicehandle, desc.iSerialNumber,
                                                   uSerialNumber, 256);
//        if (error < 0)
//        {
//            fprintf(stderr,
//                    "Ошибка: строковый дескриптор устройства "
//                    "для серийного номера не получен, код: %d.\n", error);
//            return;
//        }
    }

    // Получает информацию о устройстве
    {
        int error =
                libusb_get_string_descriptor_ascii(devicehandle, desc.iProduct,
                                                   uProduct, 256);
        if (error < 0)
        {
            fprintf(stderr,
                    "Ошибка: строковый дескриптор устройства "
                    "для названия не получен, код: %d.\n", error);
            return;
        }
    }

    // Получает информацию о производителе устройства
    {
        int error =
                libusb_get_string_descriptor_ascii(devicehandle, desc.iManufacturer,
                                                   uManufacturer, 256);
        if (error < 0)
        {
            fprintf(stderr,
                    "Ошибка: строковый дескриптор устройства "
                    "для названия производителя не получен, код: %d.\n", error);
            return;
        }
    }

    printf("Описание устройства: %s\n", uProduct);
    printf("Серийный номер устройства: %s\n", uSerialNumber);
    printf("Производитель  устройства: %s\n", uManufacturer);

    // получить конфигурацию устройства
    libusb_get_config_descriptor(dev, 0, &config);

    printf("%.2d %.4d %.6d %.6d %.3d |  |   |  |  |  |\n",  // Выводит
           (int) desc.bNumConfigurations,   // Число конфигураций
           (int) desc.bDeviceClass ? (int) desc.bDeviceClass : interdesc->bInterfaceClass,  // Класс
           // Проверяет на то, является ли класс пустым. Если да, то берет значение из дескриптора интерфейса
           desc.idVendor,   // ID Вендора (Производителя)
           desc.idProduct,  // ID Устройства
           (int) config->bNumInterfaces // Количество интерфейсов
    );

    for (int i = 0; i < (int) config->bNumInterfaces; i++)  // Проходится по всем интерфейсам
    {
        inter = &config->interface[i];      // Получает текущий интерфейс
        printf("|  |    |      |      |   " // Выводит
               "%.2d %.3d |  |  |  |\n",
               inter->num_altsetting,       // Количество альтернативных настроек
               (int) desc.bDeviceClass ? (int) desc.bDeviceClass : interdesc->bInterfaceClass   // Класс
        );

        for (int j = 0; j < inter->num_altsetting; j++) // Проходит по всем альтернативным настройкам
        {
            interdesc = &inter->altsetting[j];
            printf("|  |    |      |      |   |  |   "  // Выводит
                   "%.2d %.2d |  |\n",
                   (int) interdesc->bInterfaceNumber,   // Номер интерфейса
                   (int) interdesc->bNumEndpoints       // Число конечных точек
            );
            for (int k = 0; k < (int) interdesc->bNumEndpoints; k++)    // Проходит по всем конечным точкам
            {
                epdesc = &interdesc->endpoint[k];
                printf(                                 // Выводит
                        "|  |    |      |      |   |  |   |  |  "
                        "%.2d %.9d\n",
                        (int) epdesc->bDescriptorType,  // Тип дескриптора
                        (int) epdesc->bEndpointAddress  // Адрес конечной точки
                );
            }
        }
    }
    libusb_close(devicehandle);
    libusb_free_config_descriptor(config); // освобождает выделенную память
    printf("================================="
           "================================\n");
}