import { useState, useContext, useEffect } from "react";
import SingleColor from "../Components/Color/SingleColor";
import { clockContext } from "../Context/Context";
import RainbowColor from "../Components/Color/RainbowColor";
import WordColor from "../Components/Color/WordColor";
import HourlyColor from "../Components/Color/HourlyColor";
import ClockFace from "../Components/ClockFace";
import { useMediaQuery } from 'react-responsive';
import { useSetColorMutation, useTestColorMutation, useLazyGetConfigQuery } from "../Components/ClockAPI";
import { useTranslation } from "react-i18next"; // Add this import


function LEDColors() {
  const { t } = useTranslation(); // Add this line
  const [fullConfig, setFullConfig] = useContext(clockContext);
  const [setColor,] = useSetColorMutation();
  const [testColor,] = useTestColorMutation();
  const [config, setConfig] = useState(fullConfig?fullConfig.config : undefined);
  const [selectedOption, setSelectedOption] = useState(config ? config.colors.ledMode : "singleColor");
  const [colorConfig, setColorConfig] = useState(config && selectedOption ? { ...config.colors.ledConfig[selectedOption]  } : {});
  const [showPreview, setShowPreview] = useState(false);
  const [clockColors, setClockColors] = useState({})
  const [getConfig, { data: configData, error: configError, isLoading: configIsLoading }] = useLazyGetConfigQuery();
  const [isTestRunning, setIsTestRunning] = useState(false);

  const isTooSmall = !useMediaQuery({
    query: '(min-width: 830px)'
  })

 useEffect(() => {
     if (fullConfig !== undefined && fullConfig.config !== undefined) {
       setConfig({ ...fullConfig.config });
      }
   }, [fullConfig]); 

  function previewChangeHandler() {
    setShowPreview(!showPreview);
  }

  useEffect(() => {
    if (config !== undefined && fullConfig.colorMap === undefined) {
      setColorConfig({ ...config.colors.ledConfig[config.colors.ledMode] });
      if (config.colors.ledMode !== undefined && config.colors.ledMode !== selectedOption) {
      setSelectedOption(config.colors.ledMode);
      }
    }
  }, [config])


  function onValueChange(event) {
    setSelectedOption(event.target.value);
    setColorConfig({ ...config.colors.ledConfig[event.target.value] });
  }

  async function testConfig() {
    let body = {};
    body[selectedOption] = { ...colorConfig };
    try {
      const payload = await testColor(body).unwrap();
      setIsTestRunning(true);
    } catch { }
  }

useEffect(() => {
    if (isTestRunning) {
      const timer = setTimeout(() => {
        setIsTestRunning(false);
      }, (selectedOption==="hourlyColor"?48000:30000));
      return () => clearTimeout(timer);
    }
  }, [isTestRunning]);

  async function resetConfig() {
    try {
      const payload = await getConfig().unwrap();
      setFullConfig({...payload});
    } catch { }

  }

  async function submitConfig() {
    let body = {};
    body[selectedOption] = { ...colorConfig };
    try {
      const payload = await setColor(body).unwrap();
      setFullConfig({...payload});
    } catch { }
  }


  /**
   * Reconstruct color config and pass it to parent 
   * @param {*} configChoice
   * @todo lift to top level as a generic function?
   */
  function configHandler(configChoice) {

    setFullConfig({...fullConfig,colormap: undefined});
    var changedPar = { ...colorConfig };
    changedPar[configChoice["id"]] = configChoice.value;
    setColorConfig(changedPar);
    }

  return (
    <div>
            {isTestRunning ? <div id="overlay">
        <div className='spinner'></div> </div> : null}
      <h2>{t("colors.title")}</h2>
      <p>{t("colors.description")}</p>
      {colorConfig && <div>
        <div>
          <div>
            <button name="Test" type="button" onClick={testConfig}>{t("colors.test")}</button>
            <button name="Reset" type="button" onClick={resetConfig}>{t("colors.reset")}</button>
            <button name="Submit" type="button" onClick={submitConfig}>{t("colors.submit")}</button>
          </div>
          <input type="radio" value="singleColor" name="colorMode" checked={selectedOption === "singleColor"} onChange={onValueChange} /> <label>{t("colors.single")}</label>
          <input type="radio" value="rainbowColor" name="colorMode" checked={selectedOption === "rainbowColor"} onChange={onValueChange} /><label>{t("colors.rainbow")}</label>
          <input type="radio" value="wordColor" name="colorMode" checked={selectedOption === "wordColor"} onChange={onValueChange} /><label>{t("colors.word")}</label>
          <input type="radio" value="hourlyColor" name="colorMode" checked={selectedOption === "hourlyColor"} onChange={onValueChange} /><label>{t("colors.hourly")}</label>
        </div>
        <div>
          <div>
            {{
                          singleColor: <SingleColor colorConfig={colorConfig} onColorConfig={configHandler} setClockColors={setClockColors} />,
                          rainbowColor: <RainbowColor colorConfig={colorConfig} onColorConfig={configHandler} setClockColors={setClockColors} clockVisible={showPreview}/>,
                          wordColor: <WordColor colorConfig={colorConfig} onColorConfig={configHandler} setClockColors={setClockColors} />,
                          hourlyColor: <HourlyColor colorConfig={colorConfig} onColorConfig={configHandler} setClockColors={setClockColors} />,
            }[selectedOption]}

            {(selectedOption !== "hourlyColor" && !isTooSmall) && (
              <div>
                <div>
                  <input type="checkbox" value="preview" name="showPreview" checked={showPreview} onChange={previewChangeHandler} /><label>{t("colors.preview")}</label>
                </div>
                <div>
                  {showPreview && <ClockFace />}
                </div>
              </div>)}
          </div>
        </div>
      </div>}
    </div>
  );
}

export default LEDColors;