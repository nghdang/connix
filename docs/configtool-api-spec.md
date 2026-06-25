Endpoint Table (Final)

Method	Endpoint	Path Parameters	Request Body	Description
GET	/devices/{deviceName}	deviceName	—	Get device information
GET	/configuration-files	—	—	List configuration files
POST	/configuration-files	—	CreateConfigurationFileRequest (metadata only)	Create configuration file
GET	/configuration-files/{configurationName}	configurationName	—	Get configuration file (metadata + rules)
PATCH	/configuration-files/{configurationName}	configurationName	UpdateConfigurationFileRequest (metadata fields)	Update configuration metadata
POST	/configuration-files/import	—	{ format, file } (multipart/form-data)	Import configuration file (creates metadata + rules)
POST	/configuration-files/{configurationName}/export	configurationName	{ format }	Export configuration file (metadata + rules)
GET	/configuration-files/{configurationName}/rules	configurationName	—	List rules in configuration file
PUT	/configuration-files/{configurationName}/rules/{ruleId}	configurationName, ruleId	—	Attach rule to configuration file
DELETE	/configuration-files/{configurationName}/rules/{ruleId}	configurationName, ruleId	—	Detach rule from configuration file
GET	/rules	—	—	List rules
POST	/rules	—	CreateRuleRequest	Create rule
GET	/rules/{ruleId}	ruleId	—	Get rule
PATCH	/rules/{ruleId}	ruleId	UpdateRuleRequest	Update rule
DELETE	/rules/{ruleId}	ruleId	—	Delete rule
GET	/rules/{ruleId}/configuration-files	ruleId	—	List configuration files using rule