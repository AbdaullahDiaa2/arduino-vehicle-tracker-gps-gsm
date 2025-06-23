
#include <SoftwareSerial.h>

SoftwareSerial sim808(7, 8); // RX, TX

bool getGPS(float* lat, float* lon);
void sendSMS(String number, float lat, float lon, bool gpsAvailable = true);

void setup() {
    Serial.begin(115200);  // تهيئة الاتصال التسلسلي مع الكمبيوتر
    sim808.begin(9600);    // تهيئة الاتصال التسلسلي مع المودم SIM808

    Serial.println("✅ بدء تشغيل وحدة SIM808...");

    sim808.println("AT");  // اختبار الاتصال مع المودم
    delay(1000);
    sim808.println("AT+CMGF=1"); // وضع الرسائل النصية في وضع النص العادي
    delay(1000);
    sim808.println("AT+CNMI=1,2,0,0,0"); // استقبال الرسائل النصية بشكل مباشر
    delay(1000);
}

void loop() {
    if (sim808.available()) {
        String response = sim808.readString();
        Serial.println(response);

        // إذا تم استقبال رسالة نصية
        if (response.indexOf("+CMT:") != -1) {
            int numberStart = response.indexOf("\"") + 1;
            int numberEnd = response.indexOf("\"", numberStart);
            String senderNumber = response.substring(numberStart, numberEnd);

            Serial.print("📩 رسالة من: ");
            Serial.println(senderNumber);

            int msgStart = response.indexOf("\n", response.indexOf("+CMT:")) + 1; // استخراج نص الرسالة
            String messageText = response.substring(msgStart);
            messageText.trim();

            Serial.print("📜 نص الرسالة: ");
            Serial.println(messageText);

            // إذا كانت الرسالة تحتوي على "LOCATION"
            if (messageText.equalsIgnoreCase("LOCATION")) {
                float lat, lon;
                if (getGPS(&lat, &lon)) {
                    sendSMS(senderNumber, lat, lon);
                } else {
                    sendSMS(senderNumber, 0.0, 0.0, false);
                }
            }
        }
    }

    // عرض قيم الإحداثيات على الـ Serial Monitor بشكل مباشر
    float lat, lon;
    if (getGPS(&lat, &lon)) {
        Serial.print("📍 خط العرض: ");
        Serial.println(lat, 6);
        Serial.print("📍 خط الطول: ");
        Serial.println(lon, 6);
    } else {
        Serial.println("⚠️ لم يتم الحصول على بيانات GPS.");
    }

    delay(5000); // تأخير صغير قبل التكرار
}

bool getGPS(float* lat, float* lon) {
    sim808.println("AT+CGNSINF");
    delay(1000);

    if (sim808.available()) {
        String response = sim808.readString();
        Serial.println(response);

        // استخراج بيانات خط العرض والطول من الرد
        int startIdx = response.indexOf("+CGNSINF: 1,1,");
        if (startIdx != -1) {
            startIdx += 13; // الانتقال إلى بداية البيانات
            int firstComma = response.indexOf(',', startIdx);
            int secondComma = response.indexOf(',', firstComma + 1);
            int thirdComma = response.indexOf(',', secondComma + 1);

            if (firstComma != -1 && secondComma != -1 && thirdComma != -1) {
                String latStr = response.substring(firstComma + 1, secondComma);
                String lonStr = response.substring(secondComma + 1, thirdComma);

                // التحقق من أن البيانات صالحة (ليس ovf أو صفر)
                if (latStr.indexOf("ovf") == -1 && lonStr.indexOf("ovf") == -1) {
                    *lat = latStr.toFloat();
                    *lon = lonStr.toFloat();

                    if (*lat != 0.0 && *lon != 0.0 && *lat != -1.0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
// إرسال رسالة نصية
void sendSMS(String number, float lat, float lon, bool gpsAvailable = true) {
    String message;
    if (gpsAvailable) {
        message = "📍 موقع الجهاز:\n";
        message += "🌍 خط العرض: " + String(lat, 6) + "\n";
        message += "🌍 خط الطول: " + String(lon, 6) + "\n";
        message += "🔗 https://maps.google.com/?q=" + String(lat, 6) + "," + String(lon, 6);
    } else {
        message = "⚠️ GPS غير متوفر حاليًا. حاول مرة أخرى لاحقًا.";
    }

    sim808.println("AT+CMGF=1");
    delay(1000);
    sim808.print("AT+CMGS=\"");
    sim808.print(number);
    sim808.println("\"");
    delay(1000);
    sim808.print(message);
    delay(1000);
    sim808.write(26);  // إرسال الرسالة
    delay(5000);  // الانتظار حتى يتم إرسال الرسالة
    Serial.println("📩 تم إرسال الرسالة إلى الهاتف!");
}
