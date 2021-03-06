<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0"
        xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
        xmlns:scripts="http://www.bluedust.com/sayweather"
        xmlns:msxsl="urn:schemas-microsoft-com:xslt"
    xmlns:yweather="http://xml.weather.yahoo.com/ns/rss/1.0"
    >

<xsl:output method="text" encoding="utf-8" media-type="text/plain"/>

  <xsl:template match="/">
    <xsl:apply-templates select="rss"/>
    <xsl:apply-templates select="channel"/>
  </xsl:template>


   <xsl:template match="channel">
      <xsl:apply-templates select="item"/>
   </xsl:template>

   <xsl:template match="item">
      <xsl:apply-templates select="yweather:forecast"/>
   </xsl:template>


   <xsl:template match="yweather:forecast">
      <xsl:text>day:</xsl:text>

      <xsl:if test="@day = 'Mon'">
         <xsl:text>Monday</xsl:text>
      </xsl:if>
      <xsl:if test="@day = 'Tue'">
         <xsl:text>Tuesday</xsl:text>
      </xsl:if>
      <xsl:if test="@day = 'Wed'">
         <xsl:text>Wednesday</xsl:text>
      </xsl:if>
      <xsl:if test="@day = 'Thu'">
         <xsl:text>Thursday</xsl:text>
      </xsl:if>
      <xsl:if test="@day = 'Fri'">
         <xsl:text>Friday</xsl:text>
      </xsl:if>
      <xsl:if test="@day = 'Sat'">
         <xsl:text>Saturday</xsl:text>
      </xsl:if>
      <xsl:if test="@day = 'Sun'">
         <xsl:text>Sunday</xsl:text>
      </xsl:if>

      <xsl:text>
description:</xsl:text>
      <xsl:value-of select="@text"/>
      <xsl:text>
low:</xsl:text>
      <xsl:value-of select="@low"/>
      <xsl:text>
high:</xsl:text>
      <xsl:value-of select="@high"/>
      <xsl:text>
end:

</xsl:text>
   </xsl:template>

</xsl:stylesheet>


