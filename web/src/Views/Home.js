import React, { useEffect, useState } from "react";
import { useGetStatusQuery } from "../Components/ClockAPI";
import { useTranslation } from "react-i18next";

function Home() {
  const { t } = useTranslation();
  const { data: statusData } = useGetStatusQuery();

  const [clock, setClock] = useState(null);
  const [version, setVersion] = useState(null);

  useEffect(() => {
    if (statusData !== undefined) {
      setClock(statusData.currentTime);
      setVersion(statusData.version);
    }
  }, [statusData]);

  return (
    <div>
      <h2>{t("currentStatus")}</h2>
      <div className="section">
        <div className="labelCell">
          <label>{t("clock")} </label>
        </div>
        <div className="smallBodyCell">{clock}</div>
      </div>
      <div className="section">
        <div className="labelCell">
          <label>{t("version")} </label>
        </div>
        <div className="smallBodyCell">{version}</div>
      </div>
    </div>
  );
}

export default React.memo(Home);