<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0"
        xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
        xmlns:scripts="http://www.bluedust.com/radiofind"
        xmlns:msxsl="urn:schemas-microsoft-com:xslt"
    xmlns:yweather="http://xml.weather.yahoo.com/ns/rss/1.0"
    >

<xsl:output method="text" encoding="utf-8" media-type="text/plain"/>

<xsl:param name="SEARCH_PLACE">London</xsl:param> 

<xsl:template match="/station-list">
	<xsl:apply-templates select="station"/>
</xsl:template>

   <xsl:template match="station">
      <xsl:if test="@band = 'vhf'">
         <xsl:variable name="AREA" select="location/@area"/>

         <xsl:if test="contains($AREA, $SEARCH_PLACE)">
               <xsl:value-of select="admin/@license"/>
					<xsl:text> </xsl:text>
               <xsl:value-of select="@frequency"/>
					<xsl:text> </xsl:text>
               <xsl:value-of select="name"/>
					<xsl:text> 
</xsl:text>
          </xsl:if>
      </xsl:if>
   </xsl:template>



</xsl:stylesheet>


