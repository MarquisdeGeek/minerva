<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/applets/static/static.inc';
require_once 'warp/conf/currency/default.conf';

class Warp_Currency_Applet extends Warp_Applet
{
var $client;
var $toName, $fromName;
var $toTarget, $fromTarget;
var $currencyList;
var $cfrom, $cto;

    function Warp_Currency_Applet($caption = "Currency Calculator")
    {
       parent::__construct($caption);
		$this->valid = FALSE;
    }

    function getDirectory()
    {
        return "currency";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);
        return true;
    }

    function initSOAP(&$appMan)
    {
        global  $fromCurrency, $toCurrency;

			try {
        @$this->client = new SoapClient("http://xurrency.com/api.wsdl");
        $this->currencyList = $this->client->getCurrencies();

        $this->cfrom = $appMan->queryParameter($this, "cfrom", $fromCurrency);
        $this->cto = $appMan->queryParameter($this, "cto", $toCurrency);
        $this->cfrom = $appMan->queryParameter($this, "chfrom", $this->cfrom);
        $this->cto = $appMan->queryParameter($this, "chto", $this->cto);

        $this->fromName = $this->client->getName($this->cfrom);
        $this->toName = $this->client->getName($this->cto);
        $this->toTarget = $this->client->getValue(1, $this->cfrom, $this->cto);
        $this->fromTarget = $this->client->getValue(1, $this->cto, $this->cfrom);
		$this->valid = TRUE;

		} catch (Exception $e) {
   		print "Exceptional problem with currency handler...";
		}

    }

    function getRefreshParams(&$appMan)
    {
        return
          $appMan->getArgument($this, "cfrom", $this->cfrom)."&".
          $appMan->getArgument($this, "cto", $this->cto);
    }

    
    function getLine($i, $mult)
    {
       $line =  "<tr>";
       $line .= "<td>$i</td><td>".round($i*$mult,2)."</td>";
       $line .= "</tr>";
       return $line;
    }

    function getCurrencyLine(&$appMan, $title, $param)
    {
        $cl[] = $title;
        foreach($this->currencyList as $currency) {
           $cl[] = $appMan->getAppletLink($this, $param, $currency, $currency);
        }
        return appletUtils::getGridedContent($cl, count($cl));
    }

    function renderWindow(&$appMan)
    {
        $this->initSOAP($appMan);

		if (!$this->valid) {
			return "Unable to retrieve data from source";
		}

        $html = $this->renderInfo($appMan);

        $html .= $this->getCurrencyLine($appMan, "<b>From:</b>", "chfrom");
        $html .= $this->getCurrencyLine($appMan, "<b>To:</b>", "chto");

        $header1 = "<table border=1><tr><td>".$this->fromName."</td><td>".$this->toName."</td>";
        $header2 = "<table border=1><tr><td>".$this->toName."</td><td>".$this->fromName."</td></tr>";
        $footer = "</table>";

        $col1 = $header1;
        $col3 = $header2;
        for($i=1;$i<20;++$i) {
          $col1 .= $this->getLine($i, $this->toTarget);
          $col3 .= $this->getLine($i, $this->fromTarget);
        }
        $col1 .= $footer;
        $col3 .= $footer;

        $col2 = $header1;
        $col4 = $header2;
        for($i=20;$i<=100;$i+=10) {
          $col2 .= $this->getLine($i, $this->toTarget);
          $col4 .= $this->getLine($i, $this->fromTarget);
        }
        for($i=100;$i<=1000;$i+=100) {
          $col2 .= $this->getLine($i, $this->toTarget);
          $col4 .= $this->getLine($i, $this->fromTarget);
        }
        $col2 .= $footer;
        $col4 .= $footer;

        $left = $appMan->getView()->split2V($col1, $col2);
        $right= $appMan->getView()->split2V($col3, $col4);

        $html.= $appMan->getView()->split3V($left, "|", $right);
        return $html;
    }

    function renderPanel(&$app_man)
    {
        return "";
    }

}

?>

