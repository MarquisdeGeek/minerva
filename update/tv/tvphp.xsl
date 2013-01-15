<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:scripts="http://www.bluedust.com/tvfind"
	xmlns:msxsl="urn:schemas-microsoft-com:xslt"
    >

<xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

<xsl:param name="SEARCH_TITLE">TRUE</xsl:param>  
<xsl:param name="SEARCH_DESC">TRUE</xsl:param>  
<xsl:param name="SEARCH_TERM"></xsl:param>  

<xsl:variable name="ucase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
<xsl:variable name="lcase" select="'abcdefghijklmnopqrstuvwxyz'"/>

<!-- Match the head node -->
<xsl:template match="/">
    <xsl:apply-templates select="channel" />
</xsl:template>

<xsl:template match="channel">
   <xsl:apply-templates select="programme"/>
 
    <xsl:text disable-output-escaping="yes">$curr-&gt;_date = "</xsl:text>
        <xsl:value-of select="@date"/>
    <xsl:text>";</xsl:text>

</xsl:template>


<xsl:template match="programme">
	<xsl:call-template name="parse_results">
        <xsl:with-param name="slist" select="$SEARCH_TERM"/>
	</xsl:call-template>
</xsl:template>

<xsl:template name="search_programme">
	<xsl:param name="term"/>
	<!-- We use choose, instead of if, so that one one test branch is executed. This stops identical programmers
		from appearing twice when the search term appears in the title and description.
	-->
    <xsl:choose> 
		<!-- The magic with translate and $ucase and $lcase is necessary to perform a case insensitive search -->
        <xsl:when test = "$SEARCH_TITLE='TRUE' and contains(translate(title, $ucase, $lcase), translate($term, $ucase, $lcase))" >                        
			<xsl:call-template name="output_result"/>
		</xsl:when> 
        <xsl:when test = "$SEARCH_DESC='TRUE' and contains(translate(desc, $ucase, $lcase), translate($term, $ucase, $lcase))" >                        
			<xsl:call-template name="output_result"/>
        </xsl:when> 
    </xsl:choose> 

</xsl:template>

<xsl:template name="parse_results">
	<xsl:param name="slist"/>
		
	<xsl:if test="contains($slist,',')">
		<xsl:call-template name="search_programme">
			<xsl:with-param name="term"  select="substring-before($slist,',')"/>
		</xsl:call-template>
		
		<!-- recurse on tail -->
		<xsl:call-template name="parse_results">
			<xsl:with-param name="slist"  select="substring-after($slist,',')"/>
		</xsl:call-template>
		
	</xsl:if>
	
	<xsl:if test="not(contains($slist,','))">
		<xsl:call-template name="search_programme">
			<xsl:with-param name="term"  select="$slist"/>
		</xsl:call-template>
	</xsl:if>

</xsl:template>


<xsl:template name="output_result">
      <xsl:text disable-output-escaping="yes">$curr-&gt;add(new Programme(</xsl:text>
         "<xsl:apply-templates select="title" />",
         "<xsl:apply-templates select="desc" />",
         "<xsl:apply-templates select="start" />",
         "<xsl:apply-templates select="end" />",
         "<xsl:apply-templates select="../@id"/>"
      <xsl:text>));
</xsl:text>

</xsl:template>

<xsl:template match="title">
   <xsl:variable name="var"><xsl:value-of select="."/></xsl:variable>
   <xsl:value-of select="translate($var, '&quot;', '')"/>
</xsl:template>

<xsl:template match="desc">
   <xsl:variable name="var"><xsl:value-of select="."/></xsl:variable>
   <xsl:value-of select="translate($var, '&quot;', '')"/>
</xsl:template>

<xsl:template match="start">
	<xsl:value-of select="."/>
</xsl:template>

<xsl:template match="end">
	<xsl:value-of select="."/>
</xsl:template>

<xsl:template match="infourl">
    <xsl:text disable-output-escaping="yes">&lt;a href="</xsl:text>
	<xsl:value-of select="."/>
    <xsl:text disable-output-escaping="yes">"&gt;[]&lt;/a&gt;</xsl:text>
</xsl:template>


</xsl:stylesheet>
