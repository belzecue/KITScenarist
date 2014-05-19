#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H

#include <QString>
#include <QMap>


namespace DataStorageLayer
{
	/**
	 * @brief Класс для доступа к настройкам сценария
	 */
	class SettingsStorage
	{
	public:
		enum SettingsPlace {
			ApplicationSettings,
			ScenarioSettings
		};

	public:
		/**
		 * @brief Сохранить значение с заданным ключём
		 */
		void setValue(const QString& _key, const QString& _value, SettingsPlace _settingsPlace);

		/**
		 * @brief Сохранить карту параметров
		 */
		void setValues(const QMap<QString, QString>& _values, const QString& _valuesGroup, SettingsPlace _settingsPlace);

		/**
		 * @brief Получить значение по ключу
		 */
		QString value(const QString& _key, SettingsPlace _settingsPlace);

		/**
		 * @brief Получить группу значений
		 */
		QMap<QString, QString> values(const QString& _valuesGroup, SettingsPlace _settingsPlace);

	private:
		SettingsStorage();

		/**
		 * @brief Получить значение по умолчанию для параметра
		 */
		QString defaultValue(const QString& _key) const;

		/**
		 * @brief Значения параметров по умолчанию
		 */
		QMap<QString, QString> m_defaultValues;

		// Для доступа к конструктору
		friend class StorageFacade;
	};
}

#endif // SETTINGSSTORAGE_H