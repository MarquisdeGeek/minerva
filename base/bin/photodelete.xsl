<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
        xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
        xmlns:scripts="http://www.minervahome.net/sayweather"
        xmlns:msxsl="urn:schemas-microsoft-com:xslt"
    xmlns:yweather="http://xml.weather.yahoo.com/ns/rss/1.0"
    >

<xsl:output method="text" encoding="utf-8" media-type="text/plain"/>

  <xsl:template match="file">
     <xsl:text>obexftp -b 00:12:1c:00:18:ee -c ../Jpegfiles/ -k </xsl:text>
      <xsl:value-of select="@name"/>
	<xsl:text>
sleep 2
</xsl:text>
  </xsl:template>

</xsl:stylesheet>

