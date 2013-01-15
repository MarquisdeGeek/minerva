<?php
require_once 'system/setup.conf';
require_once 'minerva.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

class Warp_PhotoFrameEmulator_Applet extends Warp_Applet
{
var $_frame;

    function Warp_PhotoFrameEmulator_Applet($caption = "Photos")
    {
       parent::__construct($caption);
    }

	function getRefreshParams(&$appMan)
    {
        return $appMan->getArgument($this, "frame", $this->_frame);
    }

    function getDirectory()
    {
        return "simulate/photoframe";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

			$this->_frame = $appMan->queryParameter($this, "frame", "");

        return true;
    }
    
    function renderPanel(&$appMan)
    {
        return "";
    }
 
    function renderWindow(&$appMan)
    {
			$cmd = getMinervaRoot()."/bin/getalias framedir";
			$fullPath = exec($cmd);

			$frameList = array();
			if (@$handle = opendir($fullPath)) {
				while (false != ($file = readdir($handle))) {
					if ($file[0] != "." && is_dir("$fullPath/$file")) {
						$frameList[] = $file;
					}
				}
			}

			$frameID = $this->_frame;

			if ($frameID == "") {	// no frame given, list them
				$idx = 0;
				$html .= "<ul>";
				foreach($frameList AS $frameName) {
					$html .= "<li>".$appMan->getAppletLink($this, "frame", $idx, $frameName)."</li>";
					++$idx;
				}
				$html .= "</ul>";
			} else {	// frame given, show the files within it
				$fileList = array();
				$fullPath .= "/".$frameList[$frameID];

				if (@$handle = opendir($fullPath)) {
					$current = $appMan->queryParameter($this, "image", $current);

					while (false !== ($file = readdir($handle))) {
						$file = "$fullPath/$file";
						if (is_file($file)) {
							$filelist[] = $file;
						}
					}

					$current = $appMan->queryParameter($this, "image", "0");
					if ($current < 0 || $current >= sizeof($filelist)) {
						$current = 0;
					}

					$html .= "Image ".($current+1)." of ".sizeof($filelist)."<br/>";
					$img = getWebRelativePath($filelist[$current]);
					if ($img != null) {
						$link = "<img src='$img' border=0/>";
						$html .= $appMan->getAppletLink($this, "image", $current+1, $link);
					}
				}
				// Linear list of other frames
				$idx = 0;
				$html .= "<p>";
				foreach($frameList AS $frameName) {
					$html .= $appMan->getAppletLink($this, "frame", $idx, $frameName)."&nbsp;";
					++$idx;
				}

			}


        return $html;
    }
    
}

?>

