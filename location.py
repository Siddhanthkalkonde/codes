import phonenumbers
from phonenumbers import geocoder

phone_number = phonenumbers.parse("+919860006902")

print("\nPhone Numbers Locations\n")
print(geocoder.description_for_number(phone_number,"en"))


