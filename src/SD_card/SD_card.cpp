#include "../SD_card/SD_card.h"
#include "../config/config.h"



SPIClass &sdSPI = SPI;

void sdSPI_init()
{
    // Nếu dùng chung SPI với W5500, hãy đưa CS của W5500
    // và các thiết bị SPI khác lên HIGH trước khi khởi tạo SD.
    pinMode(SD_CS, OUTPUT);
    pinMode(TFT_CS, OUTPUT);
    pinMode(TOUCH_CS, OUTPUT);
    pinMode(W5500_CS, OUTPUT);

    digitalWrite(SD_CS, HIGH);
    
    digitalWrite(TFT_CS, HIGH);

    digitalWrite(TOUCH_CS, HIGH);

    digitalWrite(W5500_CS, HIGH);

    // Khởi tạo SPI: SCK, MISO, MOSI, CS
    sdSPI.begin(SCK, MISO, MOSI, SD_CS);
}

void sdSPI_test()
{
    if (!SD.begin(SD_CS, sdSPI, 4000000)) {
        Serial.println("[SD] SD Failed! Cannot innitialize SD.");
        return;
    }

    Serial.println("[SD] SD initialized succesfully!");

    // 1. Kiểm tra loại thẻ
    uint8_t type = SD.cardType();

    if (type == CARD_NONE) {
        Serial.println("[SD] SD not found.");
        return;
    }

    Serial.printf("Card type: %u\n", type);
    Serial.printf("Card size: %llu MB\n",
                    SD.cardSize() / (1024ULL * 1024ULL));

    // 2. Ghi thêm dữ liệu vào file
    File file = SD.open("/test.txt", FILE_WRITE);

    if (file) {
        file.println("ESP32 ghi du lieu SD");
        file.println("Temperature=30.5");
        file.close();
        Serial.println("Ghi file thanh cong");
    } else {
        Serial.println("Mo file de ghi that bai");
    }

    // 3. Đọc file từ đầu
    file = SD.open("/test.txt", FILE_READ);

    if (file) {
        Serial.println("--- Noi dung file ---");

        while (file.available()) {
        Serial.write(file.read());
        }

        file.close();
    } else {
        Serial.println("Mo file de doc that bai");
    }

    // 4. Kiểm tra file tồn tại
    Serial.println(SD.exists("/test.txt")
                        ? "File ton tai"
                        : "Khong co file");

    // 5. In danh sách file ở thư mục gốc
    File root = SD.open("/");
    File entry = root.openNextFile();

    while (entry) {
        Serial.printf("%s | %llu bytes\n",
                    entry.name(),
                    (unsigned long long)entry.size());

        entry.close();
        entry = root.openNextFile();
    }

    root.close();
}


bool sdSPI_write_csv(const char *path, const char *data, const char *csv_headers)
{
    // 1. Tạo các thư mục cha nếu chưa tồn tại
    char dir[128];
    snprintf(dir, sizeof(dir), "%s", path);

    char *lastSlash = strrchr(dir, '/');

    if (lastSlash != nullptr && lastSlash != dir) {
        *lastSlash = '\0';

        char *p = dir + 1;

        while ((p = strchr(p, '/')) != nullptr) {
            *p = '\0';

            if (!SD.exists(dir) && !SD.mkdir(dir)) {
                Serial.printf("[SD] Cannot create folder: %s\n", dir);
                return false;
            }

            *p = '/';
            p++;
        }

        if (!SD.exists(dir) && !SD.mkdir(dir)) {
            Serial.printf("[SD] Cannot create folder: %s\n", dir);
            return false;
        }
    }

    // 2. Kiểm tra file mới hoặc file rỗng
    bool writeHeader = true;

    if (SD.exists(path)) {
        File oldFile = SD.open(path, FILE_READ);

        if (!oldFile) {
            Serial.printf("[SD] Cannot read file: %s\n", path);
            return false;
        }

        writeHeader = (oldFile.size() == 0);
        oldFile.close();
    }

    // 3. Mở file ở chế độ ghi nối tiếp
    File file = SD.open(path, FILE_APPEND);

    if (!file) {
        Serial.printf("[SD] Cannot open file: %s\n", path);
        return false;
    }

    // 4. Ghi header nếu file mới hoặc rỗng
    if (writeHeader && csv_headers != nullptr && csv_headers[0] != '\0') {
        if (file.println(csv_headers) == 0) {
            Serial.println("[SD] Cannot write CSV header!");
            file.close();
            return false;
        }
    }

    // 5. Ghi dữ liệu
    size_t written = file.println(data);

    file.flush();
    file.close();

    if (written == 0) {
        Serial.println("[SD] CSV write failed!");
        return false;
    }

    Serial.printf("[SD] CSV written: %s\n", path);
    return true;
}



bool sdSPI_read_csv(const char *path)
{
    File file = SD.open(path, FILE_READ);

    if (!file) {
        Serial.printf("[SD] Cannot open file for reading: %s\n", path);
        return false;
    }

    Serial.printf("[SD] Reading CSV: %s\n", path);
    Serial.println("[SD] ----- CSV BEGIN -----");

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();

        if (line.length() > 0) {
            Serial.println(line);
        }
    }

    file.close();

    Serial.println("[SD] ----- CSV END -----");
    return true;
}

bool sdSPI_file_exists(const char *path)
{
    bool exists = SD.exists(path);

    Serial.printf(
        "[SD] File %s: %s\n",
        path,
        exists ? "EXISTS" : "NOT FOUND"
    );

    return exists;
}


