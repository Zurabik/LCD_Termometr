/*
 * Имя          :  n3310.c
 *
 * Описание     :  Это драйвер для графического LCD от Nokia 3310, а также его китайских клонов.
 *                 Базируется на коде библиотек написанных Sylvain Bissonnette и Fandi Gunawan:
 *                 http://www.microsyl.com/index.php/2010/03/24/nokia-lcd-library/
 *                 http://fandigunawan.wordpress.com/2008/06/18/lcd-nokia-3310-pcd8544-driver-in-winavravr-gcc/
 *                 Основные отличия между оригиналом и клоном хорошо описаны в статье от Aheir:
 *                 http://radiokot.ru/articles/29/
 *
 * Автор        :  Xander Gresolio <xugres@gmail.com>
 * Веб-страница :  http://we.easyelectronics.ru/profile/XANDER/
 *
 * Кросcплатформенность : Victor Smetaniuk <victor@breeze-soft.com>
 * Веб-страница :  http://breeze-soft.com
 *
 * Лицензия     :  GPL v3.0
 *
 * Компилятор   :  AVR Studio | IAR for ARM platform | IAR for STM8 platform
 *
 * История      :
 * Версия 1.1 (27.04.2012)
 * + Добавлена поддержка платформ ARM и STM8
 *
 * Версия 1.0 (06.08.2011)
 * + Первая версия
 * + Добавлена поддержка китайских клонов LCD Nokia 3310
 * + Полный перевод комментариев к исходному коду драйвера
 * + Таблица символов драйвера дополнена кириллицей (упрощенная Windows-1251)
 * + Добавлена функция рисования окружностей LcdCircle
 * - Исправлены ошибки в проверке корректности координат при вызове функций рисования
 * - Исправлена ошибка в функции LcdSingleBar (неверная отрисовка по y)
 */

#include "string.h"
#include "n3310.h"

// Прототипы приватных функций драйвера

static void LcdVSPI    ( byte data );
static void LcdSend    ( byte data, LcdCmdData cd );

// Глобальные переменные

// Кэш в ОЗУ 84*48 бит или 504 байта
static byte  LcdCache [ LCD_CACHE_SIZE ];

// Чтобы не обновлять весь дисплей, а лишь ту часть что изменилась,
// будем отмечать две границы кэша где произошли изменения. Затем
// можно копировать эту часть кэша между границами в ОЗУ дисплея.
static int   LoWaterMark;   // нижняя граница
static int   HiWaterMark;   // верхняя граница

// Указатель для работы с LcdCache[]
static int   LcdCacheIdx;

// Флаг изменений кэша
static byte  UpdateLcd;

#define SET_LCD_PIN(...) SET_LCD_PIN_(__VA_ARGS__) 
#define LCD_PIN_CFG(...) LCD_PIN_CFG_(__VA_ARGS__) 

//unsigned char power_down=0;			//power-down control: 0 - chip is active, 1 - chip is in PD-mode
//unsigned char addressing = 0;			//направление адресации: 0 - горизонтальная, 1- вертикальная
#define ADDRESSING 0

void LcdPwrMode (void) 				//инвертирует состояние вкл/выкл дисплея
{
	//power_down = ~power_down;
	//#ifdef china
	//LcdSend( 0x20|x_mirror<<4|y_mirror<<3|power_down<<2|addressing<<1, LCD_CMD );			//LCD Standard Commands
	//#elif
	LcdSend( 0x20|0<<2|ADDRESSING<<1, LCD_CMD );
	//#endif
}

void Lcd_off (void) 				//выкл дисплея
{
	//#ifdef china
	//LcdSend( 0x20|x_mirror<<4|y_mirror<<3|1<<2|addressing<<1, LCD_CMD );			//LCD Standard Commands
	//#elif
	LcdSend( 0x20|1<<2|ADDRESSING<<1, LCD_CMD );
	//#endif
}

void Lcd_on (void) 				//вкл дисплея
{
	//#ifdef china
	//LcdSend( 0x20|x_mirror<<4|y_mirror<<3|0<<2|addressing<<1, LCD_CMD );			//LCD Standard Commands
	//#elif
	LcdSend( 0x20|0<<2|ADDRESSING<<1, LCD_CMD );
	//#endif
}


/*
 * Имя                   :  LcdInit
 * Описание              :  Производит инициализацию порта и SPI МК, контроллера LCD
 * Аргумент(ы)           :  Нет
 * Возвращаемое значение :  Нет
 */

void LcdInit ( void )
{
    // Pull-up на вывод подключенный к reset дисплея
    SET_LCD_PIN(LCD_RST_PIN, 1);
    
    // Предконфигурация перифирии
    LCD_PRE_CFG
    
    // Устанавливаем нужные пины на выход
    LCD_PIN_CFG(LCD_DC_PIN);
    LCD_PIN_CFG(LCD_CE_PIN);
    LCD_PIN_CFG(LCD_RST_PIN);
    LCD_PIN_CFG(LCD_SDIN_PIN);
    LCD_PIN_CFG(LCD_SCLK_PIN);

    // Предконфигурация перифирии
    LCD_POST_CFG
    
    // Дергаем reset
    SET_LCD_PIN(LCD_RST_PIN, 0);
    SET_LCD_PIN(LCD_RST_PIN, 1);

    // Отключаем LCD контроллер - высокий уровень на SCE
    SET_LCD_PIN(LCD_CE_PIN, 1);
  
    // Отправляем команды дисплею
    LcdSend( 0x21, LCD_CMD ); // Включаем расширенный набор команд (LCD Extended Commands)
    LcdSend( 0xC8, LCD_CMD ); // Установка контрастности (LCD Vop)
    LcdSend( 0x06, LCD_CMD ); // Установка температурного коэффициента (Temp coefficent)
    LcdSend( 0x13, LCD_CMD ); // Настройка питания (LCD bias mode 1:48)
    LcdSend( 0x20, LCD_CMD ); // Включаем стандартный набор команд и горизонтальную адресацию (LCD Standard Commands,Horizontal addressing mode)
    LcdSend( 0x0C, LCD_CMD ); // Нормальный режим (LCD in normal mode)

    // Первичная очистка дисплея
    LcdClear();
    LcdUpdate();
}



/*
 * Имя                   :  LcdClear
 * Описание              :  Очищает дисплей. Далее необходимо выполнить LcdUpdate
 * Аргумент(ы)           :  Нет
 * Возвращаемое значение :  Нет
 */
void LcdClear ( void )
{
//    // Очистка кэша дисплея
//    int i;
//    for ( i = 0; i < LCD_CACHE_SIZE; i++ )
//    {
//        LcdCache[i] = 0x00;
//    }

    // Оптимизация от Jakub Lasinski (March 14 2009)
    memset( LcdCache, 0x00, LCD_CACHE_SIZE );
    
    // Сброс указателей границ в максимальное значение
    LoWaterMark = 0;
    HiWaterMark = LCD_CACHE_SIZE - 1;

    // Установка флага изменений кэша
    UpdateLcd = TRUE;
}



/*
 * Имя                   :  LcdUpdate
 * Описание              :  Копирует кэш в ОЗУ дисплея
 * Аргумент(ы)           :  Нет
 * Возвращаемое значение :  Нет
 */
void LcdUpdate (void)
{
    int i;

    if ( LoWaterMark < 0 )
        LoWaterMark = 0;
    else if ( LoWaterMark >= LCD_CACHE_SIZE )
        LoWaterMark = LCD_CACHE_SIZE - 1;

    if ( HiWaterMark < 0 )
        HiWaterMark = 0;
    else if ( HiWaterMark >= LCD_CACHE_SIZE )
        HiWaterMark = LCD_CACHE_SIZE - 1;

    #ifdef CHINA_LCD  // Алгоритм для китайского ЖК из нестандартным контроллером

        byte x,y;

        // 102 x 64 - таково предполагаемое разрешение буфера китайского ЖК, при чем
        // память буфера отображается на дисплей со сдвигом вверх на 3 пикселя.
        // Поэтому выводим картинку ниже - начиная с второй строки y+1, а потом
        // сдвинем вверх (опять таки фича китайца, полезная в данном случае)
                
        x = LoWaterMark % LCD_X_RES;      // Устанавливаем начальный адрес x
        LcdSend( 0x80 | x, LCD_CMD );     // относительно нижней границы LoWaterMark
        
        y = LoWaterMark / LCD_X_RES + 1;  // Устанавливаем начальный адрес y+1
        LcdSend( 0x40 | y, LCD_CMD );     // относительно нижней границы LoWaterMark

        for ( i = LoWaterMark; i <= HiWaterMark; i++ )
        {
            // передаем данные в буфер дисплея
            LcdSend( LcdCache[i], LCD_DATA );
            
            x++;                 // заодно подсчитываем координату x, чтобы вовремя перейти на новую строку
            if (x >= LCD_X_RES)  // если вышли за предел, то переходим на следующую строку (x=0; y++)
            {
                // проще говоря, чтобы верно заполнить нужную часть нестандартного буфера,
                // придется явно указывать требуемый адрес, иначе все поплывет :)
                x=0;                
                LcdSend( 0x80, LCD_CMD );
                y++;
                LcdSend( 0x40 | y, LCD_CMD );
            }
        }

        LcdSend( 0x21, LCD_CMD );    // Включаем расширенный набор команд
        LcdSend( 0x45, LCD_CMD );    // Сдвигаем картинку на 5 пикселей вверх (нестандартная команда китайца, оригинал её игнорирует)
        LcdSend( 0x20, LCD_CMD );    // Включаем стандартный набор команд и горизонтальную адресацию

    #else  // Алгоритм для оригинального дисплея

        // Устанавливаем начальный адрес в соответствии к LoWaterMark
        LcdSend( 0x80 | ( LoWaterMark % LCD_X_RES ), LCD_CMD );
        LcdSend( 0x40 | ( LoWaterMark / LCD_X_RES ), LCD_CMD );

        // Обновляем необходимую часть буфера дисплея
        for ( i = LoWaterMark; i <= HiWaterMark; i++ )
        {
            // Для оригинального дисплея не нужно следить за адресом в буфере,
            // можно просто последовательно выводить данные
            LcdSend( LcdCache[i], LCD_DATA );
        }

    #endif

    // Сброс указателей границ в пустоту
    LoWaterMark = LCD_CACHE_SIZE - 1;
    HiWaterMark = 0;

    // Сброс флага изменений кэша
    UpdateLcd = FALSE;
}


/*
 * Имя                   :  LcdVSPI
 * Описание              :  Отправляет данные в контроллер дисплея (виртуальный SPI)
 * Аргумент(ы)           :  data -> данные для отправки
 * Возвращаемое значение :  Нет
 */

static void LcdVSPI (byte data ) {
  for(unsigned int i = 0; i< 8; i++, data = data << 1){
      SET_LCD_PIN(LCD_SDIN_PIN, (data & 0x80) == 0x80);

      SET_LCD_PIN(LCD_SCLK_PIN, 1);
      SET_LCD_PIN(LCD_SCLK_PIN, 0);
  }
}

/*
 * Имя                   :  LcdSend
 * Описание              :  Отправляет данные в контроллер дисплея
 * Аргумент(ы)           :  data -> данные для отправки
 *                          cd   -> команда или данные (смотри enum в n3310.h)
 * Возвращаемое значение :  Нет
 */
static void LcdSend (byte data, LcdCmdData cd)
{
  // Включаем контроллер дисплея (низкий уровень активный)
    SET_LCD_PIN(LCD_CE_PIN, 0);

    SET_LCD_PIN(LCD_DC_PIN, cd == LCD_DATA);

    // Отправка данных в контроллер дисплея
    LcdVSPI(data);
    
    // Отключаем контроллер дисплея
    SET_LCD_PIN(LCD_CE_PIN, 1);
}



/*
 * Имя                   :  LcdContrast
 * Описание              :  Устанавливает контрастность дисплея
 * Аргумент(ы)           :  контраст -> значение от 0x00 к 0x7F
 * Возвращаемое значение :  Нет
 */
void LcdContrast ( byte contrast )
{
    LcdSend( 0x21, LCD_CMD );              // Расширенный набор команд
    LcdSend( 0x80 | contrast, LCD_CMD );   // Установка уровня контрастности
    LcdSend( 0x20, LCD_CMD );              // Стандартный набор команд, горизонтальная адресация
}



/*
 * Имя                   :  LcdGotoXYFont
 * Описание              :  Устанавливает курсор в позицию x,y относительно стандартного размера шрифта
 * Аргумент(ы)           :  x,y -> координаты новой позиции курсора. Значения: 0,0 .. 13,5
 * Возвращаемое значение :  смотри возвращаемое значение в n3310.h
 */
byte LcdGotoXYFont ( byte x, byte y )
{
    // Проверка границ
    if( x > 13 || y > 5 ) return OUT_OF_BORDER;

    //  Вычисление указателя. Определен как адрес в пределах 504 байт
    LcdCacheIdx = x * 6 + y * 84;
    return OK;
}



/*
 * Имя                   :  LcdChr
 * Описание              :  Выводит символ в текущей позиции курсора, затем инкрементирует положение курсора
 * Аргумент(ы)           :  size -> размер шрифта. Смотри enum в n3310.h
 *                          ch   -> символ для вывода
 * Возвращаемое значение :  смотри возвращаемое значение в n3310lcd.h
 */
byte LcdChr ( LcdFontSize size, byte ch )
{
    byte i, c;
    byte b1, b2;
    int  tmpIdx;

    if ( LcdCacheIdx < LoWaterMark )
    {
        // Обновляем нижнюю границу
        LoWaterMark = LcdCacheIdx;
    }

    if ( (ch >= 0x20) && (ch <= 0x7F) )
    {
        // Смещение в таблице для символов ASCII[0x20-0x7F]
        ch -= 32;
    }
    else if ( ch >= 0xC0 )
    {
        // Смещение в таблице для символов CP1251[0xC0-0xFF]
        ch -= 96;
    }
    else
    {
        // Остальные игнорируем (их просто нет в таблице для экономии памяти)
        ch = 95;
    }

    if ( size == FONT_1X )
    {
        for ( i = 0; i < 5; i++ )
        {
            // Копируем вид символа из таблицы в кэш
            LcdCache[LcdCacheIdx++] = pgm_read_byte( &(FontLookup[ch][i]) ) << 1;
        }
    }
	
	//else if ( size == FONT_BIG_I )
	// {
	//	 ch += 13;
	//	 for ( i = 0; i < 42; i++ )
	//	 {
	//		 // Копируем вид символа из таблицы в кэш
	//		 LcdCache[LcdCacheIdx++] = pgm_read_byte( &(BigFontLookup[ch][i]) ) << 1;
	//	 }
	// }
    else if ( size == FONT_2X )
    {
        tmpIdx = LcdCacheIdx - 84;

        if ( tmpIdx < LoWaterMark )
        {
            LoWaterMark = tmpIdx;
        }

        if ( tmpIdx < 0 ) return OUT_OF_BORDER;

        for ( i = 0; i < 5; i++ )
        {
            // Копируем вид символа из таблицы у временную переменную
            c = pgm_read_byte(&(FontLookup[ch][i])) << 1;
            // Увеличиваем картинку
            // Первую часть
            b1 =  (c & 0x01) * 3;
            b1 |= (c & 0x02) * 6;
            b1 |= (c & 0x04) * 12;
            b1 |= (c & 0x08) * 24;

            c >>= 4;
            // Вторую часть
            b2 =  (c & 0x01) * 3;
            b2 |= (c & 0x02) * 6;
            b2 |= (c & 0x04) * 12;
            b2 |= (c & 0x08) * 24;

            // Копируем две части в кэш
            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx + 82] = b2;
            LcdCache[tmpIdx + 83] = b2;
        }

        // Обновляем x координату курсора
        LcdCacheIdx = (LcdCacheIdx + 11) % LCD_CACHE_SIZE;
    }




    if ( LcdCacheIdx > HiWaterMark )
    {
        // Обновляем верхнюю границу
        HiWaterMark = LcdCacheIdx;
    }

    // Горизонтальный разрыв между символами
    LcdCache[LcdCacheIdx] = 0x00;
    // Если достигли позицию указателя LCD_CACHE_SIZE - 1, переходим в начало
    if(LcdCacheIdx == (LCD_CACHE_SIZE - 1) )
    {
        LcdCacheIdx = 0;
        return OK_WITH_WRAP;
    }
    // Иначе просто инкрементируем указатель
    LcdCacheIdx++;
    return OK;
}



/*
 * Имя                   :  LcdStr
 * Описание              :  Эта функция предназначена для печати строки которая хранится в RAM
 * Аргумент(ы)           :  size      -> размер шрифта. Смотри enum в n3310.h
 *                          dataArray -> массив содержащий строку которую нужно напечатать
 * Возвращаемое значение :  смотри возвращаемое значение в n3310lcd.h
 */
byte LcdStr ( LcdFontSize size, byte dataArray[] )
{
    byte tmpIdx=0;
    byte response;
    while( dataArray[ tmpIdx ] != '\0' )
    {
        // Выводим символ
        response = LcdChr( size, dataArray[ tmpIdx ] );
        // Не стоит волноваться если произойдет OUT_OF_BORDER,
        // строка будет печататься дальше из начала дисплея
        if( response == OUT_OF_BORDER)
            return OUT_OF_BORDER;
        // Увеличиваем указатель
        tmpIdx++;
    }
    return OK;
}



/*
 * Имя                   :  LcdFStr
 * Описание              :  Эта функция предназначена для печати строки которая хранится в Flash ROM
 * Аргумент(ы)           :  size    -> размер шрифта. Смотри enum в n3310.h
 *                          dataPtr -> указатель на строку которую нужно напечатать
 * Возвращаемое значение :  смотри возвращаемое значение в n3310lcd.h
 * Пример                :  LcdFStr(FONT_1X, PSTR("Hello World"));
 *                          LcdFStr(FONT_1X, &name_of_string_as_array);
 */
byte LcdFStr ( LcdFontSize size, const byte *dataPtr )
{
    byte c;
    byte response;
    for ( c = pgm_read_byte( dataPtr ); c; ++dataPtr, c = pgm_read_byte( dataPtr ) )
    {
        // Выводим символ
        response = LcdChr( size, c );
        if(response == OUT_OF_BORDER)
            return OUT_OF_BORDER;
    }

    return OK;
}



/*
 * Имя                   :  LcdPixel
 * Описание              :  Отображает пиксель по абсолютным координатам (x,y)
 * Аргумент(ы)           :  x,y  -> абсолютные координаты пикселя
 *                          mode -> Off, On или Xor. Смотри enum в n3310.h
 * Возвращаемое значение :  смотри возвращаемое значение в n3310lcd.h
 */
byte LcdPixel ( byte x, byte y, LcdPixelMode mode )
{
    int  index;
    byte  offset;
    byte  data;

    // Защита от выхода за пределы
    if ( x >= LCD_X_RES || y >= LCD_Y_RES) return OUT_OF_BORDER;

    // Пересчет индекса и смещения
    index = ( ( y / 8 ) * 84 ) + x;
    offset  = y - ( ( y / 8 ) * 8 );

    data = LcdCache[ index ];

    // Обработка битов

    // Режим PIXEL_OFF
    if ( mode == PIXEL_OFF )
    {
        data &= ( ~( 0x01 << offset ) );
    }
    // Режим PIXEL_ON
    else if ( mode == PIXEL_ON )
    {
        data |= ( 0x01 << offset );
    }
    // Режим PIXEL_XOR
    else if ( mode  == PIXEL_XOR )
    {
        data ^= ( 0x01 << offset );
    }

    // Окончательный результат копируем в кэш
    LcdCache[ index ] = data;

    if ( index < LoWaterMark )
    {
        // Обновляем нижнюю границу
        LoWaterMark = index;
    }

    if ( index > HiWaterMark )
    {
        // Обновляем верхнюю границу
        HiWaterMark = index;
    }
    return OK;
}



/*
 * Имя                   :  LcdLine
 * Описание              :  Рисует линию между двумя точками на дисплее (алгоритм Брезенхэма)
 * Аргумент(ы)           :  x1, y1  -> абсолютные координаты начала линии
 *                          x2, y2  -> абсолютные координаты конца линии
 *                          mode    -> Off, On или Xor. Смотри enum в n3310.h
 * Возвращаемое значение :  смотри возвращаемое значение в n3310lcd.h
 */
byte LcdLine ( byte x1, byte y1, byte x2, byte y2, LcdPixelMode mode )
{
    int dx, dy, stepx, stepy, fraction;
    byte response;

    // dy   y2 - y1
    // -- = -------
    // dx   x2 - x1

    dy = y2 - y1;
    dx = x2 - x1;

    // dy отрицательное
    if ( dy < 0 )
    {
        dy    = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    // dx отрицательное
    if ( dx < 0 )
    {
        dx    = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 1;
    dy <<= 1;

    // Рисуем начальную точку
    response = LcdPixel( x1, y1, mode );
    if(response)
        return response;

    // Рисуем следующие точки до конца
    if ( dx > dy )
    {
        fraction = dy - ( dx >> 1);
        while ( x1 != x2 )
        {
            if ( fraction >= 0 )
            {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;

            response = LcdPixel( x1, y1, mode );
            if(response)
                return response;

        }
    }
    else
    {
        fraction = dx - ( dy >> 1);
        while ( y1 != y2 )
        {
            if ( fraction >= 0 )
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;

            response = LcdPixel( x1, y1, mode );
            if(response)
                return response;
        }
    }

    // Установка флага изменений кэша
    UpdateLcd = TRUE;
    return OK;
}



/*
 * Имя                   :  LcdCircle
 * Описание              :  Рисует окружность (алгоритм Брезенхэма)
 * Аргумент(ы)           :  x, y   -> абсолютные координаты центра
 *                          radius -> радиус окружности
 *                          mode   -> Off, On или Xor. Смотри enum в n3310.h
 * Возвращаемое значение :  смотри возвращаемое значение в n3310lcd.h
 */
byte LcdCircle(byte x, byte y, byte radius, LcdPixelMode mode)
{
    signed char xc = 0;
    signed char yc = 0;
    signed char p = 0;

    if ( x >= LCD_X_RES || y >= LCD_Y_RES) return OUT_OF_BORDER;

    yc = radius;
    p = 3 - (radius<<1);
    while (xc <= yc)  
    {
        LcdPixel(x + xc, y + yc, mode);
        LcdPixel(x + xc, y - yc, mode);
        LcdPixel(x - xc, y + yc, mode);
        LcdPixel(x - xc, y - yc, mode);
        LcdPixel(x + yc, y + xc, mode);
        LcdPixel(x + yc, y - xc, mode);
        LcdPixel(x - yc, y + xc, mode);
        LcdPixel(x - yc, y - xc, mode);
        if (p < 0) p += (xc++ << 2) + 6;
            else p += ((xc++ - yc--)<<2) + 10;
    }

    // Установка флага изменений кэша
    UpdateLcd = TRUE;
    return OK;
}


/*
 * Имя                   :  LcdSingleBar
 * Описание              :  Рисует один закрашенный прямоугольник
 * Аргумент(ы)           :  baseX  -> абсолютная координата x (нижний левый угол)
 *                          baseY  -> абсолютная координата y (нижний левый угол)
 *                          height -> высота (в пикселях)
 *                          width  -> ширина (в пикселях)
 *                          mode   -> Off, On или Xor. Смотри enum в n3310.h
 * Возвращаемое значение :  смотри возвращаемое значение в n3310lcd.h
 */
byte LcdSingleBar ( byte baseX, byte baseY, byte height, byte width, LcdPixelMode mode )
{
    byte tmpIdxX,tmpIdxY,tmp;

    byte response;

    // Проверка границ
    if ( ( baseX >= LCD_X_RES) || ( baseY >= LCD_Y_RES) ) return OUT_OF_BORDER;

    if ( height > baseY )
        tmp = 0;
    else
        tmp = baseY - height + 1;

    // Рисование линий
    for ( tmpIdxY = tmp; tmpIdxY <= baseY; tmpIdxY++ )
    {
        for ( tmpIdxX = baseX; tmpIdxX < (baseX + width); tmpIdxX++ )
        {
            response = LcdPixel( tmpIdxX, tmpIdxY, mode );
            if(response)
                return response;

        }
    }

    // Установка флага изменений кэша
    UpdateLcd = TRUE;
    return OK;
}

/*
 * Имя                   :  LcdBarsFontNumber
 * Описание              :  Рисует цифры с помошью закрашенных прямоугольников
 * Аргумент(ы)           :  numb  -> цифра
 *                          baseХ  -> позиция Х (от 0 до 2) в числе вида ХХ.Х
 */

void LcdBarsFontNumber (char numb, byte baseX)
{
	byte tmpX;
	if (baseX==0)
	{
		tmpX=14;
	}
	if (baseX==1)
	{
		tmpX=35;
	}
	if (baseX==2)
	{
		tmpX=60;
	}
	
	switch (numb)
	{
		case 0x00: //ноль
			LcdSingleBar(tmpX+4,3,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+4,31,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+13,31,32,4,PIXEL_ON);//верт.
			LcdSingleBar(tmpX,31,32,4,PIXEL_ON);//верт.
			//LcdSingleBar(tmpX,31,32,17,PIXEL_ON);//закрасим весь прямоугольник
			//LcdSingleBar((tmpX+4),27,24,9,PIXEL_OFF);
		break;
		
		case 0x01: //единица
			//LcdSingleBar(tmpX,31,32,17,PIXEL_OFF);//очистим весь прямоугольник
			LcdSingleBar((tmpX+10),3,4,4,PIXEL_ON);
			LcdSingleBar((tmpX+13),31,32,4,PIXEL_ON);
		break;
		
		case 0x02: //двойка
			LcdSingleBar(tmpX,3,4,17,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX,17,4,17,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX,31,4,17,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+13,13,10,4,PIXEL_ON);//верт.
			LcdSingleBar(tmpX,27,10,4,PIXEL_ON);
			//LcdSingleBar(tmpX,31,32,17,PIXEL_ON);//закрасим весь прямоугольник
			//LcdSingleBar(tmpX,13,10,13,PIXEL_OFF);
			//LcdSingleBar((tmpX+4),27,10,13,PIXEL_OFF);
		break;
		
		case 0x03: //тройка
			LcdSingleBar(tmpX,3,4,17,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+4,17,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX,31,4,17,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+13,27,24,4,PIXEL_ON);//верт.
			//LcdSingleBar(tmpX,31,32,17,PIXEL_ON);//закрасим весь прямоугольник
			//LcdSingleBar(tmpX,27,24,13,PIXEL_OFF);
			//LcdSingleBar((tmpX+4),17,4,9,PIXEL_ON);
		break;

		case 0x04: //четверка
			LcdSingleBar(tmpX+4,17,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX,17,18,4,PIXEL_ON);//верт.
			LcdSingleBar(tmpX+13,31,32,4,PIXEL_ON);//верт.
			//LcdSingleBar(tmpX,31,32,17,PIXEL_ON);//закрасим весь прямоугольник
			//LcdSingleBar(tmpX,31,14,13,PIXEL_OFF);
			//LcdSingleBar((tmpX+4),13,14,9,PIXEL_OFF);
		break;
				
		case 0x05: //пятерка
			LcdSingleBar(tmpX,3,4,17,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX,17,4,17,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX,31,4,17,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+13,27,10,4,PIXEL_ON);//верт.
			LcdSingleBar(tmpX,13,10,4,PIXEL_ON);//верт.
			//LcdSingleBar(tmpX,31,32,17,PIXEL_ON);//закрасим весь прямоугольник
			//LcdSingleBar((tmpX+4),13,10,13,PIXEL_OFF);
			//LcdSingleBar((tmpX),27,10,13,PIXEL_OFF);
		break;
			
		case 0x06: //шестерка
			LcdSingleBar(tmpX+4,3,4,13,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+4,17,4,13,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+4,31,4,13,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+13,27,10,4,PIXEL_ON);//верт.
			LcdSingleBar(tmpX,31,32,4,PIXEL_ON);//верт.
			//LcdSingleBar(tmpX,31,32,17,PIXEL_ON);//закрасим весь прямоугольник
			//LcdSingleBar((tmpX+4),13,10,13,PIXEL_OFF);
			//LcdSingleBar((tmpX+4),27,10,9,PIXEL_OFF);
		break;
				
		case 0x07: //семерка
			LcdSingleBar(tmpX+4,3,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+13,31,32,4,PIXEL_ON);//верт.
			LcdSingleBar(tmpX,7,8,4,PIXEL_ON);//верт.
			//LcdSingleBar(tmpX,31,32,17,PIXEL_ON);//закрасим весь прямоугольник
			//LcdSingleBar((tmpX),31,28,13,PIXEL_OFF);
			//LcdSingleBar((tmpX),7,4,4,PIXEL_ON);
		break;
				
		case 0x08: //восьмерка
			LcdSingleBar(tmpX+4,3,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+4,17,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+4,31,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+13,31,32,4,PIXEL_ON);//верт.
			LcdSingleBar(tmpX,31,32,4,PIXEL_ON);//верт.
			//LcdSingleBar(tmpX,31,32,17,PIXEL_ON);//закрасим весь прямоугольник
			//LcdSingleBar((tmpX+4),13,10,9,PIXEL_OFF);
			//LcdSingleBar((tmpX+4),27,10,9,PIXEL_OFF);
		break;		
		
		case 0x09: //девятка
			LcdSingleBar(tmpX+4,3,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+4,17,4,9,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX,31,4,13,PIXEL_ON);//горизонт.
			LcdSingleBar(tmpX+13,31,32,4,PIXEL_ON);//верт.
			LcdSingleBar(tmpX,17,18,4,PIXEL_ON);//верт.
			//LcdSingleBar(tmpX,31,32,17,PIXEL_ON);//закрасим весь прямоугольник
			//LcdSingleBar((tmpX+4),13,10,9,PIXEL_OFF);
			//LcdSingleBar((tmpX),27,10,13,PIXEL_OFF);
		break;		
				
				
		default:
		break;
	}
	
}
/*
 * Имя                   :  LcdBars
 * Описание              :  Рисует группу закрашенных прямоугольников (в режиме PIXEL_ON)
 * Аргумент(ы)           :  data[]     -> данные которые нужно отобразить
 *                          numbBars   -> количество прямоугольников
 *                          width      -> ширина (в пикселях)
 *                          multiplier -> множитель для высоты
 * Возвращаемое значение :  смотри возвращаемое значение в n3310lcd.h
 * Примечание            :  Пожалуйста проверьте значения EMPTY_SPACE_BARS, BAR_X, BAR_Y в n3310.h
 * Пример                :  byte example[5] = {1, 2, 3, 4, 5};
 *                          LcdBars(example, 5, 3, 2);
 */
byte LcdBars ( byte data[], byte numbBars, byte width, byte multiplier )
{
    byte b;
    byte tmpIdx = 0;
    byte response;

    for ( b = 0;  b < numbBars ; b++ )
    {
        // Защита от выхода за пределы
        if ( tmpIdx > LCD_X_RES - 1 ) return OUT_OF_BORDER;

        // Расчет значения x
        tmpIdx = ((width + EMPTY_SPACE_BARS) * b) + BAR_X;

        // Рисуем один прямоугольник
        response = LcdSingleBar( tmpIdx, BAR_Y, data[b] * multiplier, width, PIXEL_ON);
        if(response == OUT_OF_BORDER)
            return response;
    }

    // Установка флага изменений кэша
    UpdateLcd = TRUE;
    return OK;

}



/*
 * Имя                   :  LcdRect
 * Описание              :  Рисует незакрашенный прямоугольник
 * Аргумент(ы)           :  x1    -> абсолютная координата x левого верхнего угла
 *                          y1    -> абсолютная координата y левого верхнего угла
 *                          x2    -> абсолютная координата x правого нижнего угла
 *                          y2    -> абсолютная координата y правого нижнего угла
 *                          mode  -> Off, On или Xor. Смотри enum в n3310.h
 * Возвращаемое значение :  смотри возвращаемое значение в n3310lcd.h
 */
byte LcdRect ( byte x1, byte y1, byte x2, byte y2, LcdPixelMode mode )
{
    byte tmpIdx;

    // Проверка границ
    if ( ( x1 >= LCD_X_RES) ||  ( x2 >= LCD_X_RES) || ( y1 >= LCD_Y_RES) || ( y2 >= LCD_Y_RES) )
        return OUT_OF_BORDER;

    if ( ( x2 > x1 ) && ( y2 > y1 ) )
    {
        // Рисуем горизонтальные линии
        for ( tmpIdx = x1; tmpIdx <= x2; tmpIdx++ )
        {
            LcdPixel( tmpIdx, y1, mode );
            LcdPixel( tmpIdx, y2, mode );
        }

        // Рисуем вертикальные линии
        for ( tmpIdx = y1; tmpIdx <= y2; tmpIdx++ )
        {
            LcdPixel( x1, tmpIdx, mode );
            LcdPixel( x2, tmpIdx, mode );
        }

        // Установка флага изменений кэша
        UpdateLcd = TRUE;
    }
    return OK;
}



/*
 * Имя                   :  LcdImage
 * Описание              :  Рисует картинку из массива сохраненного в Flash ROM
 * Аргумент(ы)           :  Указатель на массив картинки
 * Возвращаемое значение :  Нет
 */
void LcdImage ( const byte *imageData )
{
//    // Инициализация указателя кэша
//    LcdCacheIdx = 0;
//    // В пределах кэша
//    for ( LcdCacheIdx = 0; LcdCacheIdx < LCD_CACHE_SIZE; LcdCacheIdx++ )
//    {
//        // Копируем данные из массива в кэш
//        LcdCache[LcdCacheIdx] = pgm_read_byte( imageData++ );
//    }
    
    // Оптимизация от Jakub Lasinski (March 14 2009)
    memcpy_P( LcdCache, imageData, LCD_CACHE_SIZE );  // Тоже самое что и выше, но занимает меньше памяти и быстрее выполняется
    
    // Сброс указателей границ в максимальное значение
    LoWaterMark = 0;
    HiWaterMark = LCD_CACHE_SIZE - 1;

    // Установка флага изменений кэша
    UpdateLcd = TRUE;
}
